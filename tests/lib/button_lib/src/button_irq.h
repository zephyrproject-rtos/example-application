#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>


void test_a(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins);

void test_b(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins);

void test_c(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins);

void test_d(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins);