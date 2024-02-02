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

/**
 * @brief 
 * This function is used to initialized
 * a desired button declared previously
 * 
 * @param 
 * -> {gpio_dt_spec} *button, a pointer to
 * a button structure declared previously like
 * static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
 * 
 * @return
 * 0 if everything is okay, else it depends on the zephyr error
*/
int button_init(const struct gpio_dt_spec *button);

/**
 * @brief 
 * This function is used to read a button's state
 * 
 * @param 
 * -> {gpio_dt_spec} *button, a pointer to
 * a button structure declared previously like 
 * static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
 * 
 * @return 
 * the button's state : 1 if the button is pressed and 0 if the button is released
*/
int button_state(const struct gpio_dt_spec *button);

/**
 * @brief 
 * This function is used to set an irq (interruption) for the 
 * desired button
 * 
 * @param 
 * -> {gpio_dt_spec} *button, a pointer to
 * a button structure declared previously like 
 * static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
 * -> {gpio_callback_handler_t} function, the function's name you want to execute when
 * there is an interruption
 * -> {gpio_callback} *button_cb_data, a pointer to abutton structure linked to the 
 * interruption, declared previously like
 * static struct gpio_callback button3_cb;
 * -> {gpio_int_mode} edge_mode, a var to set the interruption mode, like rising_edge...
 * See the table in tests/lib/button_lib/doc/button_irq.png
 * or
 * https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.4.99-dev1/zephyr/reference/peripherals/gpio.html
 * section "GPIO interrupt configuration flags"
 * 
 * @return
 * --nothing
*/
void isr_btn_config(const struct gpio_dt_spec *button, gpio_callback_handler_t function, struct gpio_callback* button_cb_data, enum gpio_int_mode edge_mode);

#endif /* BUTTON_H */

