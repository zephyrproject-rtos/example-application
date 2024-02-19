#ifndef BOND_PERIPH_H
#define BOND_PERIPH_H

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/settings/settings.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

// You must define this variables on other parts of your program
extern struct bt_le_adv_param adv_param;

/*
    * Aim   : Redirects pairing request to function  
    * Input : Function to process pairing request. If NULL, there is default function
              User data for function
    * No output
*/
void ble_bond_init(void (*func)(const struct bt_bond_info *info, void *user_data), void *user_data);
/*
    * Aim    : Starts bonding
    * Input  : Parameter for advertising
    * Output : 0 if no error or error code
*/
int ble_bond_start(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size);

/*
    * Aim    : Stops bonding
    * Input  : Parameter for advertising
    * Output : 0 if no error or error code
*/
int ble_bond_stop(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size);

/*
    * Aim    : Activates bonding during argument time or until there isn't connection.
    * Input  : Maximum seconds of bonding
              Parameter for advertising
    * Output : 0 if no error or error code
*/
int ble_bond_duration(uint8_t duration, const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size);

/*
    * Aim    : Activates bonding until there isn't connection.
    * Input  : Parameter for advertising
    * Output : 0 if no error or error code
*/
int ble_bond_1_conn(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size);

#endif