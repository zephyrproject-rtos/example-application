/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */


#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

/* STEP 1 - Define the 128 bit UUIDs for the GATT service and its characteristics in */
/** @brief LBS Service UUID. */
#define BT_UUID_LED_VAL BT_UUID_128_ENCODE(0x00001533, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

/** @brief LED Characteristic UUID. */
#define BT_UUID_LED_LED_VAL BT_UUID_128_ENCODE(0x00001535, 0x1212, 0xefde, 0x1523, 0x785feabcd123)

#define BT_UUID_LED BT_UUID_DECLARE_128(BT_UUID_LED_VAL)
#define BT_UUID_LED_LED BT_UUID_DECLARE_128(BT_UUID_LED_LED_VAL)

/** @brief Callback type for when an LED state change is received. */
typedef void (*led_cb_t)(const bool led_state);

/** @brief Callback struct used by the LBS Service. */
struct my_led_cb {
	/** LED state change callback. */
	led_cb_t led_cb;
};

/** @brief Initialize the LBS Service.
 *
 * This function registers application callback functions with the My LBS
 * Service
 *
 * @param[in] callbacks Struct containing pointers to callback functions
 *			used by the service. This pointer can be NULL
 *			if no callback functions are defined.
 *
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int my_led_init(struct my_led_cb *callbacks);

