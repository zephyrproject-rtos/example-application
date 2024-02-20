/*
 * Copyright (c) 2018 Nordic emiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief LED Button Service (LBS) sample
 */
#include "bluetooth_lib/services/test.h"

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>





static struct my_led_cb led_cb;

/* STEP 6 - Implement the write callback function of the LED characteristic */
static ssize_t write_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
			 uint16_t len, uint16_t offset, uint8_t flags)
{


	if (len != 1U) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	if (offset != 0) {
		
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	if (led_cb.led_cb) {
		// Read the received value
		uint8_t val = *((uint8_t *)buf);

		if (val == 0x00 || val == 0x01) {
			// Call the application callback function to update the LED state
			led_cb.led_cb(val ? true : false);
		} else {
			
			return BT_GATT_ERR(BT_ATT_ERR_VALUE_NOT_ALLOWED);
		}
	}

	return len;
}


/* LED Button Service Declaration */
/* STEP 2 - Create and add the MY LBS service to the Bluetooth LE stack */
BT_GATT_SERVICE_DEFINE(my_led_svc, BT_GATT_PRIMARY_SERVICE(BT_UUID_LED),
		       /* STEP 4 - Create and add the LED characteristic. */
		       BT_GATT_CHARACTERISTIC(BT_UUID_LED_LED, BT_GATT_CHRC_WRITE,
					      BT_GATT_PERM_WRITE, NULL, write_led, NULL),

);
/* A function to register application callbacks for the LED and Button characteristics  */
int my_led_init(struct my_led_cb *callbacks)
{
	if (callbacks) {
		led_cb.led_cb = callbacks->led_cb;
	}

	return 0;
}
