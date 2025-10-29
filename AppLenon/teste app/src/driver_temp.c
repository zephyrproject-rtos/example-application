#include "driver_temp.h"
#include <zephyr/logging/log.h>

// Registra um módulo de log para este arquivo, o que nos permitirá imprimir
// mensagens de depuração de forma organizada.
LOG_MODULE_REGISTER(at30tse_driver, CONFIG_LOG_DEFAULT_LEVEL);

/**
 * @brief Converte o valor bruto de 12 bits do sensor para graus Celsius.
 *
 * @param raw_temp O valor de 16 bits lido do registrador do sensor.
 * @return A temperatura calculada em graus Celsius.
 */
float convert_raw_to_celsius(uint16_t raw_temp)
{
    // A resolução do sensor, conforme o datasheet, é 0.0625 °C por passo.
    const float RESOLUTION = 0.0625f;

    // O dado de temperatura real está nos 12 bits mais significativos.
    // Primeiro, isolamos esses 12 bits.
    int16_t temp_12bit = (int16_t)(raw_temp >> 4);

    // O valor é negativo se o 12º bit (bit de sinal) for 1.
    // Usamos a máscara 0x0800 para verificar.
    if ((temp_12bit & 0x0800) != 0) {
        // Se for negativo, precisamos "estender o sinal" para que o
        // compilador entenda que é um número negativo de 16 bits.
        // Isso é feito preenchendo os 4 bits mais altos com 1s.
        temp_12bit |= 0xF000;
    }

    // Finalmente, multiplicamos o valor com sinal pela resolução.
    return (float)temp_12bit * RESOLUTION;
}

/*
 * NOTA: As outras funções do driver, como as que irão ler os dados
 * via I2C, serão adicionadas aqui mais tarde, quando o hardware
 * estiver disponível.
 *
 * Exemplo de como ficaria:
 *
 * int at30tse_read_temp(const struct device *dev, float *temp_c) {
 * uint8_t i2c_data[2];
 * // ... código para ler do barramento i2c ...
 * uint16_t raw_value = (i2c_data[0] << 8) | i2c_data[1];
 * *temp_c = convert_raw_to_celsius(raw_value);
 * return 0;
 * }
 */