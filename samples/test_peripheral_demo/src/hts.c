/** @file
 *  @brief HTS Service sample
 */

/*
 * Copyright (c) 2020 SixOctets Systems
 * Copyright (c) 2019 Aaron Tsui <aaron.tsui@outlook.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "hts.h"

static uint8_t indicating;
static uint8_t envoi;

//Indicate params
static struct bt_gatt_indicate_params ind_params;

//Variable pour stocker les valeurs de la characteristic BT_UUID_GATT_DO du service BT_UUID_UDS
uint8_t attr_value[] = {0};

K_SEM_DEFINE(ind_activated, 0, 1);


/* --------------- Fonction qui verifie si la configuration du CCC change et l'indique ------------*/
static void ccc_cfg_changed(const struct bt_gatt_attr *attr,
				 uint16_t value)
{
	if (value == BT_GATT_CCC_INDICATE) {
        // Le client a activé les indications
        printk("Indications activées par le client\n");
		envoi = 1;
		//k_sem_give(&ind_activated);
    } else if (value == BT_GATT_CCC_NOTIFY) {
        // Le client a activé les notifications
        printk("Notifications activées par le client\n");
		envoi = 1;
		//k_sem_give(&ind_activated);
    } else {
        // Le client a désactivé les indications et les notifications
        printk("Indications et notifications désactivées par le client\n");
		envoi = 0;
    }
}



/* ----- Fonction de callback de la characteristic BT_UUID_GATT_DO qui s'exécute lors d'un read d'une centrale ----- */
ssize_t read_fonction_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset)
{
	int i;
	size_t to_copy = MIN(len, sizeof(attr_value) - offset);
    if (to_copy > 0) {
        memcpy(buf, attr_value + offset, to_copy);
		printk("Valeur lue par la centrale : ");
		for (i=0; i<to_copy; i++){
			printk("%d ", ((uint8_t*)buf)[i]);
		}
		printk("\n");
        return to_copy;
    } else {
        return 0;
    }
};


/* ----- Fonction de callback de la characteristic BT_UUID_GATT_DO qui s'exécute lors d'un write d'une centrale ----- */
ssize_t write_fonction_callback(struct bt_conn *conn,
					     const struct bt_gatt_attr *attr,
					     const void *buf, uint16_t len,
					     uint16_t offset, uint8_t flags)
{
	int i;
	size_t to_copy = MIN(len, sizeof(attr_value) - offset);
    if (to_copy > 0) {
        memcpy(attr_value + offset, buf, to_copy);
		printk("Valeur écrite par la centrale : ");
		for (i=0; i<to_copy; i++){
			printk("%d ", attr_value[i]);
		}
		printk("\n");
        return to_copy;
    } else {
        return 0;
    }
};


/* --------------- Declaration du servicequ'on utilise pour l'envoi des données ---------------*/
BT_GATT_SERVICE_DEFINE(userdata_svc,
	//Déclaration du premier service
	//L'argument est l'UUID du service à utiliser. La liste des UUID est dans uuid.h
	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),

	//Déclaration de la caractéristique, permissions, callback etc...
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, attr_value),													   
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(NULL),

	//Déclaration de la caractéristique, permissions, callback etc...
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, attr_value),													   
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CPF(NULL),

	// //Déclaration de la caractéristique, permissions, callback etc...
	// BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, attr_value),													   
	// BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	// BT_GATT_CPF(NULL),

	// //Déclaration de la caractéristique, permissions, callback etc...
	// BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, attr_value),													   
	// BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	// BT_GATT_CPF(NULL),

	// //Déclaration de la caractéristique, permissions, callback etc...
	// BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, attr_value),													   
	// BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	// BT_GATT_CPF(NULL),

	// //Déclaration de la caractéristique, permissions, callback etc...
	// BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , read_fonction_callback, write_fonction_callback, attr_value),													   
	// BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	// BT_GATT_CPF(NULL),


);



/* ------- Callback de l'indicate, s'exécute lorsque l'indicate est commencé -------------*/
static void indicate_cb(struct bt_conn *conn,
			struct bt_gatt_indicate_params *params, uint8_t err)
{
	printk("Indication %s\n", err != 0U ? "fail" : "success");
}


/* -------------Callback de l'indicate, s'exécute lorsque l'indicate est fini -----------*/
static void indicate_destroy(struct bt_gatt_indicate_params *params)
{
	//Une fois l'indicate fini on remet indicating=0 afin de pouvoir refaire un indicate.
	printk("Indication complete\n");
	indicating = 0U;
}

/* ---------------- Fonction qui se charge de l'indicate, appelée dans le main ------------------ */
void valeur_indicate(struct bt_conn *conn)
{		
	//k_sem_take(&ind_activated, K_FOREVER);
	while(1){
			if (indicating) {
				return;
			}
			// Mise à jour des paramètres
			ind_params.attr = &userdata_svc.attrs[1];
			ind_params.func = indicate_cb;
			ind_params.destroy = indicate_destroy;
			ind_params.data = attr_value;
			ind_params.len = sizeof(attr_value);

			// On envoie la nouvelle valeur avec un indicate
			if (bt_gatt_indicate(NULL, &ind_params) == 0) {
				indicating = 1U;
			}
			k_msleep(1000);

			ind_params.attr = &userdata_svc.attrs[5];
			// On envoie la nouvelle valeur avec un indicate
			if (bt_gatt_indicate(NULL, &ind_params) == 0) {
				indicating = 1U;
			}
			k_msleep(1000);

		
	}
}
