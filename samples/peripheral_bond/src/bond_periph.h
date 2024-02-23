/**
 * \file bond_periph.h
 * \brief peripheral bond library
 * \author Johan P.
*/


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

/**
 * 
 * \brief Redirects pairing request to function  
 * \param func Function to process pairing request. If NULL, there is default function
 * \param user_data User data for function
*/
void ble_bond_init(void (*func)(const struct bt_bond_info *info, void *user_data), void *user_data);

/**
 * \brief Start bonding
 * \param ad structure for advertising
 * \param  ad_size size of ad
 * \param  sd structure for advertising
 * \param  sd_size size of sd
 * \return 0 if no error or error code
*/
int ble_bond_start(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size);

/**
 * \brief Stop bonding
 * \param ad structure for advertising
 * \param  ad_size size of ad
 * \param  sd structure for advertising
 * \param  sd_size size of sd
 * \return 0 if no error or error code
*/
int ble_bond_stop(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size);

/**
 * \brief Activates bonding during argument time or until there isn't connection.
 * \param duration maximum seconds of bonding
 * \param ad structure for advertising
 * \param  ad_size size of ad
 * \param  sd structure for advertising
 * \param  sd_size size of sd
 * \return 0 if no error or error code
*/
int ble_bond_duration(uint8_t duration, const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size);

/**
 * \brief Activates bonding until there isn't connection.
 * \param ad structure for advertising
 * \param  ad_size size of ad
 * \param  sd structure for advertising
 * \param  sd_size size of sd
 * \return 0 if no error or error code
*/
int ble_bond_1_conn(const struct bt_data* ad, int ad_size, const struct bt_data* sd, int sd_size);

#endif