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

bt_gatt_attr_read_func_t read_cb(void);

static uint8_t envoi;
static uint8_t indicating;
static uint8_t val_envoyee = 100;
static struct bt_gatt_indicate_params ind_params;

// Fonction qui verifie si la configuration de la connexion est bien en indicate. 1 si oui
static void ccc_cfg_changed(const struct bt_gatt_attr *attr,
				 uint16_t value)
{
	envoi = (value == BT_GATT_CCC_INDICATE) ? 1 : 0; 
}


//Indique que l'indicate a été réussie ou non
static void indicate_cb(struct bt_conn *conn,
			struct bt_gatt_indicate_params *params, uint8_t err)
{
	printk("Inddddication %s\n", err != 0U ? "fail" : "success");
}


//Indique que l'indicate a été effectuée
static void indicate_destroy(struct bt_gatt_indicate_params *params)
{
	//Une fois l'indicate fini on remet indicating=0 afin de pouvoir refaire un indicate.
	printk("Indication complete\n");
	indicating = 0U;
}



/* Declaration du service */
/*arguments : (nom, uuid, ) */
BT_GATT_SERVICE_DEFINE(hts_svc,
	//Primary Service Declaration Macro.
	//L'argument est l'UUID du service à utiliser. La liste des UUID est dans uuid.h
	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),

	//Characteristic and Value Declaration Macro.
	//arg 1 = UUID de la characteristic
	//arg 2 = propriétés de l'attribut de la characteristic
	//arg 3 = permission de lire
	//arg 4 = read callback
	//arg 5 = write callback
	//arg 6 = user data
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , NULL, NULL, NULL),

	//Client Characteristic Configuration Declaration Macro.
	//arg 1 = fonction de callback si la configuration a changé														   
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	/* more optional Characteristics */
);



// Dans le main on arrive sur cette fonction qui regarde si la valeur à envoyer à changé puis l'envoie si oui.
void valeur_indicate(void)
{		
	printk("Indicate\n");
	if (envoi){
		if (indicating) {
			return;
		}

		val_envoyee += 1;

		// Mise à jour des paramètres
		ind_params.attr = &hts_svc.attrs[2];
		ind_params.func = indicate_cb;
		ind_params.destroy = indicate_destroy;
		ind_params.data = &val_envoyee;
		ind_params.len = sizeof(val_envoyee);

		// On envoie les nouvelles valeurs en Bluetooth
		if (bt_gatt_indicate(NULL, &ind_params) == 0) {
			indicating = 1U;

		}
	}
}
