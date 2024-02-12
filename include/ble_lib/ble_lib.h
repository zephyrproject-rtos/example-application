#ifndef BLE_LIB_H
#define BLE_LIB_H

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/settings/settings.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/bluetooth/services/hrs.h>
#include <zephyr/bluetooth/services/ias.h>

int ble_advertise_start_conn(   const struct bt_data* ad,
                                size_t ad_size, 
                                const struct bt_data* sd,
                                size_t sd_size);

int ble_advertise_start_nconn(  const struct bt_data* ad,
                                size_t ad_size, 
                                const struct bt_data* sd,
                                size_t sd_size);


#endif