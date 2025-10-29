/**
 Teste Bare-Metal do BNO055 no SAM D21 (SAMD21G18A)
 PINAGEM (Baseado no SAM D21 Xplained Pro):
 - I2C: SERCOM2
 - PA08: SDA
 - PA09: SCL
 - UART: SERCOM3
 - PA22: TX (EDBG Virtual COM)
 - PA23: RX (EDBG Virtual COM)
 */

#include <samd21.h>
#include <stdbool.h>

/* ============================================================================
 * PINOUT E CONSTANTES DE PERIFÉRICOS
 * ============================================================================*/

// --- I2C ---
#define I2C_SERCOM          SERCOM2
#define I2C_SERCOM_GCLK_ID  SERCOM2_GCLK_ID_CORE
#define I2C_APBCMASK        PM_APBCMASK_SERCOM2
#define I2C_PORT            PORT->Group[0]
#define I2C_PIN_SDA         8
#define I2C_PIN_SCL         9
#define I2C_PMUX_SDA        PORT_PMUX_PMUXE_D // PA08 é par -> PMUXE
#define I2C_PMUX_SCL        PORT_PMUX_PMUXO_D // PA09 é ímpar -> PMUXO
#define I2C_PMUX_REG_IDX    (I2C_PIN_SDA / 2) // PMUX[4]

// --- UART ---
#define UART_SERCOM         SERCOM3
#define UART_SERCOM_GCLK_ID SERCOM3_GCLK_ID_CORE
#define UART_APBCMASK       PM_APBCMASK_SERCOM3
#define UART_PORT           PORT->Group[0]
#define UART_PIN_TX         22
#define UART_PIN_RX         23
#define UART_PMUX_TX        PORT_PMUX_PMUXE_C // PA22 é par -> PMUXE
#define UART_PMUX_RX        PORT_PMUX_PMUXO_C // PA23 é ímpar -> PMUXO
#define UART_PMUX_REG_IDX   (UART_PIN_TX / 2) // PMUX[11]

// --- BNO055 ---
#define BNO055_I2C_ADDR         (0x28)
#define BNO055_REG_CHIP_ID      (0x00)
#define BNO055_REG_OPR_MODE     (0x3D)
#define BNO055_REG_EUL_HEADING_LSB (0x1A)
#define BNO055_MODE_CONFIG      (0x00)
#define BNO055_MODE_NDOF        (0x0C)

/* ============================================================================
 * CONFIGURAÇÃO DO HARDWARE (Clocks, Pinos, SysTick)
 * ============================================================================*/

volatile uint32_t g_ticks_ms = 0;
void SysTick_Handler(void) {
    g_ticks_ms++;
}

void delay_ms(uint32_t ms) {
    uint32_t start = g_ticks_ms;
    while ((g_ticks_ms - start) < ms);
}

void init_systick(void) {
    SysTick_Config(48000000 / 1000);
    NVIC_SetPriority(SysTick_IRQn, 3);
}

void init_clocks(void) {
    // 1. Configura a NVM (Flash) para 1 Wait-State (necessário para 48MHz)
    NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;

    // 2. Carrega valores de calibração do DFLL48M (Fábrica)
    uint32_t coarse = (*((uint32_t *)FUSES_DFLL48M_COARSE_CAL_ADDR) & FUSES_DFLL48M_COARSE_CAL_Msk) >> FUSES_DFLL48M_COARSE_CAL_Pos;
    uint32_t fine = (*((uint32_t *)FUSES_DFLL48M_FINE_CAL_ADDR) & FUSES_DFLL48M_FINE_CAL_Msk) >> FUSES_DFLL48M_FINE_CAL_Pos;

    // 3. Configura o DFLL48M (Digital Frequency Locked Loop)
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0) | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_GENEN;
    while (GCLK->STATUS.bit.SYNCBUSY);
    
    // Remove proteção de escrita dos registradores do DFLL
    SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_ENABLE; 
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
    
    // Seta os valores de calibração
    SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE(coarse) | SYSCTRL_DFLLVAL_FINE(fine);
    
    // Habilita o DFLL em modo "Closed Loop"
    SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE | SYSCTRL_DFLLCTRL_ENABLE;
    
    // 4. Configura GCLK0 como fonte DFLL48M (48MHz)
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0) | 
                        GCLK_GENCTRL_SRC_DFLL48M |
                        GCLK_GENCTRL_IDC | // Melhora o duty cycle
                        GCLK_GENCTRL_GENEN; // Habilita o GCLK0
    while (GCLK->STATUS.bit.SYNCBUSY);

    // 5. Habilita clocks da APBC para os periféricos SERCOM
    PM->APBCMASK.reg |= I2C_APBCMASK | UART_APBCMASK;

    // 6. Roteia GCLK0 (48MHz) para o I2C_SERCOM
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(I2C_SERCOM_GCLK_ID) |
                        GCLK_CLKCTRL_GEN_GCLK0 |
                        GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.bit.SYNCBUSY);

    // 7. Roteia GCLK0 (48MHz) para o UART_SERCOM
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(UART_SERCOM_GCLK_ID) |
                        GCLK_CLKCTRL_GEN_GCLK0 |
                        GCLK_CLKCTRL_CLKEN;
    while (GCLK->STATUS.bit.SYNCBUSY);
}

void init_pins(void) {
    // --- Configuração dos Pinos UART ---
    // Habilita o multiplexador de pinos
    UART_PORT.PINCFG[UART_PIN_TX].bit.PMUXEN = 1;
    UART_PORT.PINCFG[UART_PIN_RX].bit.PMUXEN = 1;
    // Seta a função 'C' (SERCOM3) para os pinos PA22/PA23
    UART_PORT.PMUX[UART_PMUX_REG_IDX].reg = UART_PMUX_TX | UART_PMUX_RX;

    // --- Configuração dos Pinos I2C ---
    // Habilita o multiplexador de pinos
    I2C_PORT.PINCFG[I2C_PIN_SDA].bit.PMUXEN = 1;
    I2C_PORT.PINCFG[I2C_PIN_SCL].bit.PMUXEN = 1;
    // Seta a função 'D' (SERCOM2) para os pinos PA08/PA09
    I2C_PORT.PMUX[I2C_PMUX_REG_IDX].reg = I2C_PMUX_SDA | I2C_PMUX_SCL;
}

/* ============================================================================
 * DRIVER UART (PARA PRINTF)
 * ============================================================================*/

void init_uart(void) {
    // Reseta o SERCOM
    UART_SERCOM->USART.CTRLA.bit.SWRST = 1;
    while (UART_SERCOM->USART.SYNCBUSY.bit.SWRST);

    // Configura o modo: 8N1, 16x oversampling, LSB first
    UART_SERCOM->USART.CTRLA.reg = SERCOM_USART_CTRLA_MODE_USART_INT_CLK | // Clock Interno
                                   SERCOM_USART_CTRLA_DORD_LSB |     // LSB primeiro
                                   SERCOM_USART_CTRLA_SAMPR(1);      // 16x oversampling

    // Configura os PADS (PAD[0] = TX, PAD[1] = RX)
    UART_SERCOM->USART.CTRLA.reg |= SERCOM_USART_CTRLA_TXPO(0) | SERCOM_USART_CTRLA_RXPO(1);
    
    // Habilita TX e RX, 8 bits
    UART_SERCOM->USART.CTRLB.reg = SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_CHSIZE(0);
    while(UART_SERCOM->USART.SYNCBUSY.bit.CTRLB);

    // Calculo do Baud Rate (115200)
    // BAUD = 65536 * (1 - (16 * 115200) / 48000000) = 63019
    UART_SERCOM->USART.BAUD.reg = 63019;

    // Habilita o SERCOM UART
    UART_SERCOM->USART.CTRLA.bit.ENABLE = 1;
    while(UART_SERCOM->USART.SYNCBUSY.bit.ENABLE);
}

void uart_putc(char c) {
    // Espera o registrador de dados estar pronto
    while (!UART_SERCOM->USART.INTFLAG.bit.DRE);
    UART_SERCOM->USART.DATA.reg = c;
}

void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

/* ============================================================================
 * DRIVER I2C (BLOCKING / POLLING)
 * ============================================================================*/

void init_i2c(void) {
    // Reseta o SERCOM
    I2C_SERCOM->I2CM.CTRLA.bit.SWRST = 1;
    while (I2C_SERCOM->I2CM.SYNCBUSY.bit.SWRST);
    
    // Configura o modo: I2C Master
    I2C_SERCOM->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;

    // Calculo do Baud Rate (100kHz Standard Mode)
    // BAUD = (F_GCLK / (2 * F_SCL)) - 1 = (48MHz / 200kHz) - 1 = 239
    I2C_SERCOM->I2CM.BAUD.reg = 239;

    // Habilita o SERCOM I2C
    I2C_SERCOM->I2CM.CTRLA.bit.ENABLE = 1;
    while(I2C_SERCOM->I2CM.SYNCBUSY.bit.ENABLE);
    
    // Força o estado do barramento para IDLE
    I2C_SERCOM->I2CM.STATUS.bit.BUSSTATE = 1;
}

bool i2c_write_read(uint8_t addr, 
                    uint8_t* tx_buf, uint16_t tx_len,
                    uint8_t* rx_buf, uint16_t rx_len) 
{
    // --- Fase de Escrita (se houver) ---
    if (tx_len > 0) {
        // Envia START e Endereco de Escrita
        I2C_SERCOM->I2CM.ADDR.reg = (addr << 1) | 0; // '0' = Write
        while(!I2C_SERCOM->I2CM.INTFLAG.bit.MB); // Espera ACK (Master on Bus)
        if (I2C_SERCOM->I2CM.STATUS.bit.RXNACK) {
            I2C_SERCOM->I2CM.CTRLB.bit.CMD = 3; // STOP
            return false;
        }

        // Envia os bytes do buffer de escrita
        for (uint16_t i = 0; i < tx_len; i++) {
            I2C_SERCOM->I2CM.DATA.reg = tx_buf[i];
            while(!I2C_SERCOM->I2CM.INTFLAG.bit.MB);
            if (I2C_SERCOM->I2CM.STATUS.bit.RXNACK) {
                I2C_SERCOM->I2CM.CTRLB.bit.CMD = 3; // STOP
                return false;
            }
        }
    }

    // --- Fase de Leitura (se houver) ---
    if (rx_len > 0) {
        // Envia (REPEATED) START e Endereco de Leitura
        I2C_SERCOM->I2CM.ADDR.reg = (addr << 1) | 1; // '1' = Read
        while(!I2C_SERCOM->I2CM.INTFLAG.bit.SB); // Espera ACK (Slave on Bus)
        if (I2C_SERCOM->I2CM.STATUS.bit.RXNACK) {
            I2C_SERCOM->I2CM.CTRLB.bit.CMD = 3; // STOP
            return false;
        }

        // Le os bytes
        for (uint16_t i = 0; i < rx_len; i++) {
            // Se for o ultimo byte, prepara um NACK
            if (i == (rx_len - 1)) {
                I2C_SERCOM->I2CM.CTRLB.bit.ACKACT = 1;
            }
            
            while(!I2C_SERCOM->I2CM.INTFLAG.bit.SB);
            rx_buf[i] = I2C_SERCOM->I2CM.DATA.reg;
        }
        
        // Envia NACK e STOP (ou apenas STOP se rx_len=1)
        I2C_SERCOM->I2CM.CTRLB.bit.CMD = 3; 
    } else {
        // Se nao houver leitura, apenas envia o STOP
        I2C_SERCOM->I2CM.CTRLB.bit.CMD = 3;
    }

    return true;
}

bool bno_read_regs(uint8_t reg, uint8_t* rx_buf, uint16_t len) {
    return i2c_write_read(BNO055_I2C_ADDR, &reg, 1, rx_buf, len);
}

bool bno_write_reg(uint8_t reg, uint8_t val) {
    uint8_t tx_buf[2] = {reg, val};
    return i2c_write_read(BNO055_I2C_ADDR, tx_buf, 2, NULL, 0);
}

/* ============================================================================
 * LÓGICA PRINCIPAL DA APLICAÇÃO
 * ============================================================================*/

int main(void) {
    uint8_t chip_id = 0;
    uint8_t euler_data[6];
    
    // Inicializa Hardware
    init_clocks();
    init_pins();
    init_systick();
    init_uart();
    init_i2c();

    uart_puts("--- Teste BNO055 (Bare-Metal SAM D21) ---\r\n");
    uart_puts("Hardware inicializado. Aguardando sensor...\r\n");
    delay_ms(100);

    // Verifica Conexão (Le Chip ID)
    if (!bno_read_regs(BNO055_REG_CHIP_ID, &chip_id, 1)) {
        uart_puts("ERRO: Falha na comunicacao I2C.\r\n");
        while(1);
    }
    
    if (chip_id == 0xA0) {
        uart_puts("Sucesso! Chip ID BNO055 (0xA0) lido.\r\n");
    } else {
        uart_puts("ERRO: Chip ID incorreto.\r\n");
        while(1);
    }

    // Configura o Modo NDOF (Fusão de 9 Eixos)
    uart_puts("Configurando modo NDOF...\r\n");
    bno_set_mode(BNO055_MODE_CONFIG);
    delay_ms(20);
    bno_set_mode(BNO055_MODE_NDOF);
    delay_ms(20);
    uart_puts("Modo NDOF configurado. Iniciando leitura...\r\n\r\n");

    // Loop de Leitura
    while (1) {
        if (bno_read_regs(BNO055_REG_EUL_HEADING_LSB, euler_data, 6)) {
            // Converte os dados brutos (int16_t, LSB primeiro)
            int16_t h_raw = ((int16_t)euler_data[1] << 8) | euler_data[0];
            
            // Imprime apenas uma string para mostrar que a leitura foi bem-sucedida
            // (Imprimir floats exigiria uma biblioteca de printf completa)
            uart_puts("Leitura OK. Raw Heading (LSB): ");
            
            // Gambiarra para imprimir o byte LSB como dois caracteres hex
            char hex_lookup[] = "0123456789ABCDEF";
            uart_putc(hex_lookup[(euler_data[0] >> 4) & 0x0F]);
            uart_putc(hex_lookup[euler_data[0] & 0x0F]);
            uart_puts("\r\n");
            
        } else {
            uart_puts("Falha na leitura...\r\n");
        }
        delay_ms(100);
    }
}

