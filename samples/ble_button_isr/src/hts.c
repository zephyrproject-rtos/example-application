#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "hts.h"


static uint8_t simulate_htm;
static uint8_t indicating;
static struct bt_gatt_indicate_params ind_params;

//static uint16_t val_envoyee = 0xABCD; //Valeur qu'on souhaite envoyer


// Regarde si la valeur à envoyer à changer
static void htmc_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value){
	simulate_htm = (value == BT_GATT_CCC_INDICATE) ? 1 : 0;  //le changement de la valeur de la caractéristique sera indicate
}


//Indique que l'indicate a été réussie ou non
static void indicate_cb(struct bt_conn *conn,
						struct bt_gatt_indicate_params *params, uint8_t err){
	printk("Indication %s\n", err != 0U ? "fail" : "success");
}


//Indique que l'indicate a été effectuée
static void indicate_destroy(struct bt_gatt_indicate_params *params){
	//Une fois l'indicate fini on remet indicating=0 afin de pouvoir refaire un indicate.
	printk("Indication complete\n");
	indicating = 0U;
}



/* Declaration du service USER_DATA */
BT_GATT_SERVICE_DEFINE(usr_data_svc,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),														//User Data Service
	BT_GATT_CHARACTERISTIC(	BT_UUID_GATT_DO,													//Digital Output
							BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,     //Permissions
							BT_GATT_PERM_READ , 
							NULL, 
							NULL, 
							NULL),

	//Client Characteristic Configuration Declaration Macro.														   
	BT_GATT_CCC(htmc_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);



// Dans le main on arrive sur cette fonction qui regarde si la valeur à envoyer à changé puis l'envoie si oui.
void hts_indicate(uint8_t* val)
{
	if (simulate_htm) {
		static uint8_t htm[5];

		if (indicating) {
			return;
		}

		goto gatt_indicate;

gatt_indicate:
		// Mise à jour des paramètres
		ind_params.attr = &usr_data_svc.attrs[2];
		ind_params.func = indicate_cb;
		ind_params.destroy = indicate_destroy;
		ind_params.data = val;//&htm;
		ind_params.len = sizeof(*val);//sizeof(htm);
		printk("\n VALEUR VAL : %d",*val);
		// On envoie les nouvelles valeurs en Bluetooth
		if (bt_gatt_indicate(NULL, &ind_params) == 0) {
			indicating = 1U;
		}
	}
}