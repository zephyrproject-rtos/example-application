#include <button_lib/button_lib.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

int button_init(const struct gpio_dt_spec *button) {
    int ret;

    if (!gpio_is_ready_dt(button)) {
        printk("Error: button device %s is not ready\n", button->port->name);
        return 0;
    }

    ret = gpio_pin_configure_dt(button, GPIO_INPUT);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, button->port->name, button->pin);
        return 0;
    }

    ret = gpio_pin_interrupt_configure_dt(button, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
        printk("Error %d: failed to configure interrupt on %s pin %d\n", ret, button->port->name, button->pin);
        return 0;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button->pin));
    gpio_add_callback(button->port, &button_cb_data);
    printk("Set up button at %s pin %d\n", button->port->name, button->pin);

    return 1;
}

int button_state(const struct gpio_dt_spec *button) {
    int val = gpio_pin_get_dt(button);
    if (val >= 0) {
        return val;
    }
    return -1;
}
