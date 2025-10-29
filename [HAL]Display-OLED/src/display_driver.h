#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
#define DISPLAY_BUFFER_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT / 8)

typedef struct {
    uint8_t pixels[DISPLAY_HEIGHT][DISPLAY_WIDTH / 8];
} DisplayBuffer;

void oled_init(void);
void oled_clear(void);
void oled_set_pixel(int x, int y, bool value);
void oled_update(void (*spi_func)(uint8_t*, size_t));
void oled_draw_text(int x, int y, const char* str);

uint8_t* oled_get_buffer(void);

#endif // DISPLAY_DRIVER_H