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

int button_init(const struct gpio_dt_spec *button);
int button_state(const struct gpio_dt_spec *button);
void isr_btn_config(const struct gpio_dt_spec *button, gpio_callback_handler_t function, struct gpio_callback* button_cb_data, enum gpio_int_mode edge_mode);

#endif /* BUTTON_H */
