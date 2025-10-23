// display_driver.c
#include "display_driver.h"
#include <string.h>

static uint8_t buffer[DISPLAY_BUFFER_SIZE];

void oled_clear(void) {
    memset(buffer, 0, sizeof(buffer));
}

void oled_set_pixel(int x, int y, bool value) {
    if (x < 0 || x >= DISPLAY_WIDTH || y < 0 || y >= DISPLAY_HEIGHT) return;
    int byte_index = (y / 8) * DISPLAY_WIDTH + x;
    uint8_t bit_mask = 1 << (y % 8);
    if (value)
        buffer[byte_index] |= bit_mask;
    else
        buffer[byte_index] &= ~bit_mask;
}

uint8_t* oled_get_buffer(void) {
    return buffer;
}

void oled_update(void (*spi_func)(uint8_t*, size_t)) {
    spi_func(buffer, DISPLAY_BUFFER_SIZE);
}