
#include <led_lib/led_lib.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
/*
static const struct gpio_dt_spec leds[] =  {GPIO_DT_SPEC_GET(LED0_NODE, gpios),
                                            GPIO_DT_SPEC_GET(LED1_NODE, gpios),
                                            GPIO_DT_SPEC_GET(LED2_NODE, gpios),
                                            GPIO_DT_SPEC_GET(LED3_NODE, gpios)
                                            };*/


int main() {
    
    led_init(&led0);

    while (1) {
        led_blink(&led0,125);
    }

    return 0;
}
