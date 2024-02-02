#include "button_irq.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>


void test_a(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("test a\n");
}

void test_b(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("test_b\n");
}

void test_c(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("test_c\n");
}

void test_d(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("test_d\n");
}