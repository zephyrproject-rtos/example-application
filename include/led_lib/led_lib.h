/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef LED_H
#define LED_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

int led_init(const struct gpio_dt_spec *led);

int led_on(const struct gpio_dt_spec *led);

int led_off(const struct gpio_dt_spec *led);

int led_blink(const struct gpio_dt_spec *led, int time);

int led_blink_duration(const struct gpio_dt_spec *led, int time, int duree);

int leds_init(const struct gpio_dt_spec *led);

int leds_off(const struct gpio_dt_spec *led);

int leds_on(const struct gpio_dt_spec *led);

int leds_circular(const struct gpio_dt_spec *led, int time);

int leds_blink(const struct gpio_dt_spec *led, int time);

int leds_blink_duration(const struct gpio_dt_spec *led, int time, int duree);


#endif /* BUTTON_H */
