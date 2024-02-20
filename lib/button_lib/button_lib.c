#include <button_lib/button_lib.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>


int button_init(const struct gpio_dt_spec *button) {
    int ret;
    //Check if the button can be set
    if (!gpio_is_ready_dt(button)) {
        printk("Error: button device %s is not ready\n", button->port->name);
        return 0;
    }
    //Set button as an input (it can't be an output !!)
    ret = gpio_pin_configure_dt(button, GPIO_INPUT);
    if (ret != 0) {
        printk("Error %d: failed to configure %s pin %d\n", ret, button->port->name, button->pin);
        return 0;
    }
    return 1;
}

bool button_state(const struct gpio_dt_spec *button) {
    bool val = gpio_pin_get_dt(button);//Read the button's state
    return val;
}

void isr_btn_config(const struct gpio_dt_spec *button, gpio_callback_handler_t function, 
                    struct gpio_callback* button_cb_data, enum gpio_int_mode edge_mode){
    gpio_pin_interrupt_configure_dt(button, edge_mode);//Set the isr in the approppriate mode (rising, falling...)
    gpio_init_callback(button_cb_data, function, BIT(button->pin));//Init the callback (function call)
	gpio_add_callback(button->port, button_cb_data);//Add the callback in the isr list
}