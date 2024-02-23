#ifndef SERVICE_H
#define SERVICE_H

#include<ble_lib/ble_lib.h>

bool button_led0;
struct Ble_Data button_led0_struct={
	.data=&button_led0,
	.size=sizeof(button_led0)
};

bool button_led1;
struct Ble_Data button_led1_struct={
	.data=&button_led1,
	.size=sizeof(button_led1)
};

bool button_led2;
struct Ble_Data button_led2_struct={
	.data=&button_led2,
	.size=sizeof(button_led2)
};

bool button_led3;
struct Ble_Data button_led3_struct={
	.data=&button_led3,
	.size=sizeof(button_led3)
};


BT_GATT_SERVICE_DEFINE(userdata_svc,

	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ| BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_fonction_callback, write_fonction_callback, &button_led0_struct),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ| BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ  | BT_GATT_PERM_WRITE, read_fonction_callback, write_fonction_callback, &button_led1_struct),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ| BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, &button_led2_struct),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ| BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_fonction_callback, write_fonction_callback, &button_led3_struct),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(NULL),
);


#endif