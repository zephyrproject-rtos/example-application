#ifndef DATA_H
#define DATA_H

#include<ble_lib/ble_lib.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
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

bool button0_val;

struct Ble_Data button0_struct={
	.data=&button0_val,
	.size=sizeof(bool)
};

char led0_cb[4];
bool led1_cb;
bool led2_cb;
bool led3_cb;


struct Ble_Data led0_cb_info = {
    .size = sizeof(led0_cb),
    .data = led0_cb,
};


// Déclaration du Format de Profil Caractéristique (GATT_CPF)
static const struct bt_gatt_cpf type1 = {
	.format = 0x01,        /* bool */
	.exponent = 0x0,
	.unit = 0x2700,        /* Unitless */
	.name_space = 0x01,    /* Bluetooth SIG */
	.description = 0x0001, /* "first characteristic" */
};

BT_GATT_SERVICE_DEFINE(userdata_svc,

	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),
	
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ, BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &button0_struct),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ, BT_GATT_PERM_WRITE | BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &led0_cb_info),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),
/*
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, &led1_cb),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, &led2_cb),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, &led3_cb),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),*/
);

#endif