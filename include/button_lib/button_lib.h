/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef BUTTON_H
#define BUTTON_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

int button_init(const struct gpio_dt_spec *button);
int button_state(const struct gpio_dt_spec *button);

#endif /* BUTTON_H */
