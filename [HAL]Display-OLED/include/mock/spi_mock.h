#ifndef SPI_MOCK_H
#define SPI_MOCK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Buffer interno para armazenar os dados enviados
#define MOCK_SPI_BUFFER_SIZE 1024
extern uint8_t spi_mock_buffer[MOCK_SPI_BUFFER_SIZE];
extern size_t spi_mock_index;
extern bool spi_mock_overflow;

// Função mock que simula envio SPI
void mock_spi_write(uint8_t* data, size_t len);

// Reseta o buffer do mock
void spi_mock_reset(void);

// Retorna quantidade de bytes escritos no buffer
size_t mock_spi_get_bytes_written(void);

// Verifica se houve overflow na última operação
bool mock_spi_has_overflow(void);

#endif // SPI_MOCK_H
