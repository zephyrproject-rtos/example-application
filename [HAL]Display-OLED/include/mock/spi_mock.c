#include "spi_mock.h"
#include <string.h>

uint8_t spi_mock_buffer[MOCK_SPI_BUFFER_SIZE];
size_t spi_mock_index = 0;
bool spi_mock_overflow = false;

void mock_spi_write(uint8_t* data, size_t len) {

    if (data == NULL || len == 0) {
        return;
    }
    
    if (spi_mock_index + len > MOCK_SPI_BUFFER_SIZE) {
        spi_mock_overflow = true;
        return;
    }
    
    memcpy(&spi_mock_buffer[spi_mock_index], data, len);
    spi_mock_index += len;
}

void spi_mock_reset(void) {
    spi_mock_index = 0;
    spi_mock_overflow = false;
    memset(spi_mock_buffer, 0, MOCK_SPI_BUFFER_SIZE);
}

size_t mock_spi_get_bytes_written(void) {
    return spi_mock_index;
}

bool mock_spi_has_overflow(void) {
    return spi_mock_overflow;
}
