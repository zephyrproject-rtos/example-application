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
static struct gpio_callback button0_cb;


void button_interrupt(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}




int main() {
    button_init(&button0);
    gpio_pin_interrupt_configure_dt(&button0,GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button0_cb, button_interrupt, BIT(button0.pin));
	gpio_add_callback(button0.port, &button0_cb);
    while (1) {
        int val1 = button_state(&button0);
        printk("\nValue btn1: %d",val1);
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
