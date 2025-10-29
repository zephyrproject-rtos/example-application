#include "../src/display_driver.h"
#include "../include/test_framework/tester_framework.h"
#include "../include/mock/spi_mock.h"
#include <stdio.h>
#include <string.h>

static char* test_clear(void) {
    uint8_t* buffer = oled_get_buffer();
    
    oled_set_pixel(0, 0, true);
    oled_clear();
    
    for(size_t i = 0; i < DISPLAY_BUFFER_SIZE; i++) {
        verifica("Falha em test_clear: buffer não foi zerado completamente", 
                 buffer[i] == 0);
    }
    
    return NULL;
}

static char* test_set_pixel(void) {
    uint8_t* buffer = oled_get_buffer();
    
    oled_clear();
    oled_set_pixel(0, 0, true);
    int byte_index = 0;
    verifica("Falha em test_set_pixel: pixel não foi setado para true", 
             buffer[byte_index] == 1);
    
    oled_set_pixel(0, 0, false);
    verifica("Falha em test_set_pixel: pixel não foi setado para false", 
             buffer[byte_index] == 0);
    
    return NULL;
}

static char* test_update_display_spi(void) {
    oled_clear();
    oled_set_pixel(0, 0, true);
    oled_set_pixel(5, 10, true);

    spi_mock_reset();
    oled_update(mock_spi_write);

    verifica("Falha em test_update_display_spi: pixel (0,0) não enviado corretamente",
             spi_mock_buffer[0] & 0x01);

    int byte_index = (10 / 8) * DISPLAY_WIDTH + 5;
    uint8_t bit_mask = 1 << (10 % 8);
    verifica("Falha em test_update_display_spi: pixel (5,10) não enviado corretamente",
             spi_mock_buffer[byte_index] & bit_mask);

    return NULL;
}

static char* executa_todos_testes(void) {
    executa_teste(test_clear);
    executa_teste(test_set_pixel);
    executa_teste(test_update_display_spi);
    return NULL;
}

int main() {
    framework_inicia_contagem();
    
    char* resultado = executa_todos_testes();
    
    if (resultado != NULL) {
        printf("ERRO: %s\n", resultado);
        return 1;
    }
    
    printf("TODOS OS TESTES PASSARAM!\n");
    printf("Total de testes executados: %d\n", framework_get_contagem());
    
    return 0;
}
