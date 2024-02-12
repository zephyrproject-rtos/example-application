/* main.c - Application main entry point */

/*
 * Copyright (c) 2019 Aaron Tsui <aaron.tsui@outlook.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

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
#include <zephyr/bluetooth/services/bas.h>

#include "hts.h"

/*------------- Déclaration d'une structure bt_conn pour récupérer -----------
----------------------les informations de connexion ------------------------*/
static struct bt_conn *info_conn;

/*---------- Données présentes dans les données de l'advertising ----------*/
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
};

/*---------- Fonctions de callback pour les évenements bluetooth ---------*/
static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err 0x%02x)\n", err);
	} else {
		printk("Connected\n");
		/*Une fois connectés on récupère les infos de connexion*/
		info_conn = conn;
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

/* -------------------------- Fonctions bluetooth --------------------------------*/
static void bt_ready(void)
{
	int err;

	printk("Bluetooth initialized\n");

	/*On advertise en mode connectable*/
	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");
}

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



int main(void)
{
	int err;

	/*Activation du bluetooth*/
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	/*Démarrage de l'advertising*/
	bt_ready();

	/*Definition de la structure de callback liée a l'authentification*/
	bt_conn_auth_cb_register(&auth_cb_display);

	/*Envoi de la mesure en indicate*/
	while(1){
		k_sleep(K_MSEC(5000));
		/* Indication de la valeur toutes les 5 secondes*/
		valeur_indicate(info_conn);
	};

	return 0;
}
