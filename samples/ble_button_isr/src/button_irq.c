#include "button_irq.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

extern uint8_t flag;

void isr_ble(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	flag=!flag;
}