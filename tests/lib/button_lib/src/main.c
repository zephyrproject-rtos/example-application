#include <button_lib/button_lib.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS	1
#define SW0_NODE	DT_ALIAS(sw0)


static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});




int main() {
    button_init(&button0);

    while (1) {
        int val1 = button_state(&button0);
        printk("\nValue btn1: %d",val1);
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}
