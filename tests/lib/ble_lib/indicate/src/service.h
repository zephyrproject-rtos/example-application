#ifndef SERVICE_H
#define SERVICE_H

#include<ble_lib/ble_lib.h>

bool button0_val;
struct Ble_Data button0_struct={
	.data=&button0_val,
	.size=sizeof(button0_val)
};

char led0_cb[26];
struct Ble_Data led0_cb_info = {
    .size = sizeof(led0_cb),
    .data = led0_cb,
};

uint8_t age_cb=24;
struct Ble_Data age_cb_info = {
    .size = sizeof(age_cb),
    .data = &age_cb,
};

char name_cb[30]="Julien";
struct Ble_Data name_cb_info = {
    .size = sizeof(name_cb),
    .data = name_cb,
};

char email_cb[]="quentin89.caldeira@gmail.com";
struct Ble_Data email_cb_info = {
    .size = sizeof(email_cb),
    .data = email_cb,
};




// Déclaration du Format de Profil Caractéristique (GATT_CPF)
static const struct bt_gatt_cpf type1 = {
	.format = 0x01,        /* bool */
	.exponent = 0x0,
	.unit = 0x2700,        /* Unitless */
	.name_space = 0x01,    /* Bluetooth SIG */
	.description = 0x0001, /* "first characteristic" */
};

BT_GATT_SERVICE_DEFINE(gpios_svc,

	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ, BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &button0_struct),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(NULL),
/*
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_AO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ, BT_GATT_PERM_WRITE | BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &led0_cb_info),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ, BT_GATT_PERM_WRITE | BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &led0_cb_info),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ, BT_GATT_PERM_WRITE | BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &led0_cb_info),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),*/
);
/*
BT_GATT_SERVICE_DEFINE(userdata_svc,

	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),
	
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_AGE, BT_GATT_CHRC_READ, BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &age_cb_info),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ),
	BT_GATT_CPF(NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_FIRST_NAME, BT_GATT_CHRC_READ, BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &name_cb_info),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ),
	BT_GATT_CPF(NULL),

	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_EMAIL, BT_GATT_CHRC_READ, BT_GATT_PERM_READ , read_fonction_callback, write_fonction_callback, &email_cb_info),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ),
	BT_GATT_CPF(NULL),
);
*/

#endif