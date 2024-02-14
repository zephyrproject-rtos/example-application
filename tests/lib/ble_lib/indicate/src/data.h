#ifndef DATA_H
#define DATA_H


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


/*---------- Fonctions de callback pour les évenements bluetooth ---------*/
static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err 0x%02x)\n", err);
	} else {
		printk("Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason 0x%02x)\n", reason);
}
/*-------------------------------------------------------------------------------*/

/*----------On définit une structure de callback pour les event connected ----------
-------------- et disconnected, avec les fonctions de callback associées ---------*/
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};



static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing cancelled: %s\n", addr);
}
/* ---------------------------------------------------------------------------------*/

/* --- Définition fonction de callback pour l'évenement authentification cancel ----*/
static struct bt_conn_auth_cb auth_cb_display = {
	/*Si cancel de l'appairage, alors on exécute la fonction auth_cancel*/
	.cancel = auth_cancel,
};



// Déclaration du Format de Profil Caractéristique (GATT_CPF)
static const struct bt_gatt_cpf type1 = {
	.format = 0x01,        /* uint8 */
	.exponent = 0x0,
	.unit = 0x27AD,        /* Percentage */
	.name_space = 0x01,    /* Bluetooth SIG */
	.description = 0x0106, /* "main" */
};

static const struct bt_gatt_cpf type2 = {
	.format = 0x06,        /* uint8 */
	.exponent = 0x0,
	.unit = 0x27C2,        /* Percentage */
	.name_space = 0x01,    /* Bluetooth SIG */
	.description = 0x0106, /* "main" */
};

BT_GATT_SERVICE_DEFINE(userdata_svc,

	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),
	
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , NULL, NULL, NULL),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type1),
/*
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , NULL, NULL, NULL),
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(&type2),*/
);

#endif