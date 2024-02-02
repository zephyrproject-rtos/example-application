#include <button_lib/button_lib.h>
#include "button_irq.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS	1

#define SW0_NODE	DT_ALIAS(sw0)
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button0_cb;
#define SW1_NODE	DT_ALIAS(sw1)
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios, {1});
static struct gpio_callback button1_cb;
#define SW2_NODE	DT_ALIAS(sw2)
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET_OR(SW2_NODE, gpios, {2});
static struct gpio_callback button2_cb;
#define SW3_NODE	DT_ALIAS(sw3)
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET_OR(SW3_NODE, gpios, {3});
static struct gpio_callback button3_cb;

int main() {
    button_init(&button0);
    isr_btn_config(&button0, test_a, &button0_cb, GPIO_INT_EDGE_RISING);
    button_init(&button1);
    isr_btn_config(&button1, test_b, &button1_cb, GPIO_INT_EDGE_BOTH);
    button_init(&button2);
    isr_btn_config(&button2, test_c, &button2_cb, GPIO_INT_EDGE_FALLING);
    button_init(&button3);
    isr_btn_config(&button3, test_d, &button3_cb, GPIO_INT_EDGE_BOTH);
    while (1);
    return 0;
}
