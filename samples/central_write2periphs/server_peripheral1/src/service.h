#ifndef SERVICE_H
#define SERVICE_H

#include<ble_lib/ble_lib.h>

uint8_t mydata;
struct Ble_Data mydata_received={
	.data=&mydata,
	.size=sizeof(mydata)
};



BT_GATT_SERVICE_DEFINE(userdata_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ| BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, &mydata_received),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(NULL),
);


#endif