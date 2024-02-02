#include <button_lib/button_lib.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

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
    return 1;
}

int button_state(const struct gpio_dt_spec *button) {
    int val = gpio_pin_get_dt(button);
    if (val >= 0) {
        return val;
    }
    return -1;
}

void isr_btn_config(const struct gpio_dt_spec *button, gpio_callback_handler_t function, struct gpio_callback* button_cb_data, enum gpio_int_mode edge_mode){
    gpio_pin_interrupt_configure_dt(button, edge_mode);
    gpio_init_callback(button_cb_data, function, BIT(button->pin));
	gpio_add_callback(button->port, button_cb_data);
	printk("Set up button at %s pin %d\n", button->port->name, button->pin);
}