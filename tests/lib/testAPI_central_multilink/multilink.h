/* multilink.h - Header for multilink */

/*
* Created by : Billy Phung (billy.phung@outlook.fr)
* for a Final Year Engineering School Project in collaboration with Open
*/


#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>

#include "button_lib/button_lib.h"
#include "led_lib/led_lib.h"

#define CONFIG_BT_MAX_CONN 62

#define SCAN_INTERVAL 0x0640 /* 1000 ms */
#define SCAN_WINDOW   0x0030 /* 30 ms */
#define INIT_INTERVAL 0x0010 /* 10 ms */
#define INIT_WINDOW   0x0010 /* 10 ms */
#define CONN_INTERVAL 0x0320 /* 1000 ms */
#define CONN_LATENCY  0
#define CONN_TIMEOUT  MIN(MAX((CONN_INTERVAL * 125 * \
			       MAX(CONFIG_BT_MAX_CONN, 6) / 1000), 10), 3200)







/**
 * @brief 
 * This function is used to scan for nearby devices and connect to it by default.
 * 
 * @param 
 * -> {bt_le_scan_cb_t} cb, callback function activated when a device is found.
 * by default NULL : 
 * If NULL, then connect to device with strong enough RSSI.  
 * 
 * @return
 * an error index. 0 if no error.
*/
int ble_scan_start(bt_le_scan_cb_t cb);



/**
 * @brief 
 * This function is used to enable Bluetooth functionnalities on the board.
 * 
 * @param 
 * None. 
 * 
 * @return
 * an error index. 0 if no error.
*/
int ble_enable();



/**
 * @brief 
 * This function prints every connected devices' MAC adress in the terminal
 * 
 * @param 
 * None. 
 * 
 * @return
 * -
*/
void ble_showconn();