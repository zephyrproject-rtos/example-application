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
#include "button_lib/button_lib.h"
#include "led_lib/led_lib.h"

//Déclaration du boutons 
#define SW0_NODE	DT_ALIAS(sw0)
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback isr_btn1;

//LED
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

//Déclaration sémaphore
K_SEM_DEFINE(button_1, 0, 1);

/*---Define the interrupts function for the bouton-------------------------------------------------------------*/
void button_pressed1(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
	printk("Button 1 pressed at\n");
	k_sem_give(&button_1);
	attr_value[0] = 1;
}

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
	//Init LED
	err = led_init(&led1);
	if (!err){
		printk("LED init failed (err %d)\n", err);
		return 0;
	}

	//Init btn1
	err = button_init(&button1); 
	if (!err){
		printk("Button init failed (err %d)\n", err);
		return 0;
	}

	//Init isr_btn1                                
    isr_btn_config(&button1, button_pressed1, &isr_btn1, GPIO_INT_EDGE_RISING);   

	/*Activation du bluetooth*/
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	/*Démarrage de l'advertising*/
	bt_ready();

	/*Definition de la structure de callback liée a l'authentification*/
	err = bt_conn_auth_cb_register(&auth_cb_display);
	if (err) {
		printk("Bt connexion callback register failed (err %d)\n", err);
		return 0;
	}

	k_sem_take(&button_1, K_FOREVER);
	valeur_indicate(info_conn);
	
	while(attr_value[0] != 3){
		k_sleep(K_MSEC(1));
	}

	led_on(&led1);

	return 0;
}
