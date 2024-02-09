/* main.c - Application main entry point */

/*
 * Copyright (c) 2020 SixOctets Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stddef.h>
#include <errno.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

static int scan_start(void);

static struct bt_conn *default_conn;
static struct bt_uuid_16 discover_uuid = BT_UUID_INIT_16(0);
static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_subscribe_params subscribe_params;


//Read params
static struct bt_gatt_read_params read_params;
//Structure bt_conn pour récupérer les informations de connexion
static struct bt_conn *info_conn;
// Variable utilisée pour les changements de la configuration du CCC
static uint8_t envoi;
//Variable pour stocker les valeurs de la characteristic BT_UUID_GATT_DO du service BT_UUID_UDS
static uint8_t attr_value[] = { 0 };


/* --------------- Fonction qui verifie si la configuration du CCC change et l'indique ------------*/
static void ccc_cfg_changed(const struct bt_gatt_attr *attr,
				 uint16_t value)
{
	envoi = (value == BT_GATT_CCC_INDICATE) ? 1 : 0; 
	if (value == BT_GATT_CCC_INDICATE) {
        // Le serveur a activé les indications
        printk("Indications activées par le serveur\n");
    } else if (value == BT_GATT_CCC_NOTIFY) {
        // Le serveur a activé les notifications
        printk("Notifications activées par le serveur\n");
    } else {
        // Le serveur a désactivé les indications et les notifications
        printk("Indications et notifications désactivées par le serveur\n");
    }
}


/* --------------- Declaration du servicequ'on utilise pour l'envoi des données ---------------*/
BT_GATT_SERVICE_DEFINE(userdata_svc,
	//Déclaration du premier service
	//L'argument est l'UUID du service à utiliser. La liste des UUID est dans uuid.h
	BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),

	//Déclaration de la caractéristique, permissions, callback etc...
	BT_GATT_CHARACTERISTIC(BT_UUID_GATT_DO, BT_GATT_CHRC_INDICATE | BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE , NULL, NULL, attr_value),

	//Client Characteristic Configuration Declaration Macro.													   
	BT_GATT_CCC(ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);


/*--------------Fonction de callback qui s'éxecute lorsqu'une valeur est indicate -------------
------------------et qui fait l'affichage de la valeur reçue, et la stocke ------------------*/
static uint8_t val_received(struct bt_conn *conn,
			   struct bt_gatt_subscribe_params *params,
			   const void *data, uint16_t length)
{
	int i;
	if (!data) {
		printk("[UNSUBSCRIBED]\n");
		params->value_handle = 0U;
		return BT_GATT_ITER_STOP;
	}

	printk("Donnée reçue :");
	for (i=0; i<length; i++){
		attr_value[i] = ((uint8_t*)data)[i];
		printk("%d ", attr_value[i]);
	}
	printk("\n");
	return BT_GATT_ITER_CONTINUE;
}


/*----- Fonction de callback qui compare les UUID du device trouvé avec l'UUID qu'on cherche -----*/
static uint8_t discover_func(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr,
			     struct bt_gatt_discover_params *params)
{
	int err;

	if (!attr) {
		printk("Discover complete\n");
		(void)memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	}

	printk("[ATTRIBUTE] handle %u\n", attr->handle);
	

	/*Si l'UUID qu'on a dans discover est celui du service qu'on souhaite, on rentre*/
	if (!bt_uuid_cmp(discover_params.uuid, BT_UUID_UDS)) {

		memcpy(&discover_uuid, BT_UUID_GATT_DO, sizeof(discover_uuid));
		discover_params.uuid = &discover_uuid.uuid;
		discover_params.start_handle = attr->handle + 1;
		discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;

		err = bt_gatt_discover(conn, &discover_params);
		if (err) {
			printk("Discover failed (err %d)\n", err);
		}
	} 
	
	/*Si l'UUID qu'on a dans discover est celui de la characteristic qu'on souhaite, on rentre*/
	else if (!bt_uuid_cmp(discover_params.uuid, BT_UUID_GATT_DO)) {
		memcpy(&discover_uuid, BT_UUID_GATT_CCC, sizeof(discover_uuid));
		discover_params.uuid = &discover_uuid.uuid;
		discover_params.start_handle = attr->handle + 2;
		discover_params.type = BT_GATT_DISCOVER_DESCRIPTOR;
		subscribe_params.value_handle = bt_gatt_attr_value_handle(attr);

		err = bt_gatt_discover(conn, &discover_params);
		if (err) {
			printk("Discover failed (err %d)\n", err);
		}
	} 
	
	else {
		subscribe_params.notify = val_received;
		subscribe_params.value = BT_GATT_CCC_INDICATE;
		subscribe_params.ccc_handle = attr->handle;

		err = bt_gatt_subscribe(conn, &subscribe_params);
		if (err && err != -EALREADY) {
			printk("Subscribe failed (err %d)\n", err);
		} else {
			printk("[SUBSCRIBED]\n");
		}
		
		return BT_GATT_ITER_STOP;
	}

	return BT_GATT_ITER_STOP;
}


/*----------- Fonction qui s'exécute quand l'event "bt connected" est reçu --------------*/
static void connected(struct bt_conn *conn, uint8_t conn_err)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err;

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (conn_err) {
		printk("Failed to connect to %s (%u)\n", addr, conn_err);

		bt_conn_unref(default_conn);
		default_conn = NULL;

		scan_start();
		return;
	}

	printk("Connected: %s\n", addr);

	/*Récupération de la connexion*/
	info_conn = conn;
	
	if (conn == default_conn) {
		memcpy(&discover_uuid, BT_UUID_UDS, sizeof(discover_uuid));
		discover_params.uuid = &discover_uuid.uuid;
		discover_params.func = discover_func;
		discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
		discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
		discover_params.type = BT_GATT_DISCOVER_PRIMARY;

		err = bt_gatt_discover(default_conn, &discover_params);
		if (err) {
			printk("Discover failed(err %d)\n", err);
			return;
		}
	}
}


/*--- Fonction de callback qui s'exécute lorsqu'on a trouvé un device auquel on peut se connecter ---*/
/*--- Test si ce device utilise le service qu'on recherche, avec lequel on souhaite communiquer -----*/
static bool eir_found(struct bt_data *data, void *user_data)
{
	bt_addr_le_t *addr = user_data;
	int i;

	printk("[AD]: %u data_len %u\n", data->type, data->data_len);

	switch (data->type) {
	case BT_DATA_UUID16_SOME:
	case BT_DATA_UUID16_ALL:
		if (data->data_len % sizeof(uint16_t) != 0U) {
			printk("AD malformed\n");
			return true;
		}

		for (i = 0; i < data->data_len; i += sizeof(uint16_t)) {
			const struct bt_uuid *uuid;
			uint16_t u16;
			int err;

			/*On récupère chaque 16 bits reçus un par un, on les copie dans u16*/
			memcpy(&u16, &data->data[i], sizeof(u16));

			/*On recupère l'UUID formé avec ces 16 bits*/
			uuid = BT_UUID_DECLARE_16(sys_le16_to_cpu(u16));

			/*On compare si cet UUID le même que celui qu'on veut*/
			if (bt_uuid_cmp(uuid, BT_UUID_UDS)) {
				/*Si ce n'est pas le même on passe à l'itération suivante*/
				continue;
			}

			/*Si c'est le même, on arrête le bt_scan*/
			err = bt_le_scan_stop();
			if (err) {
				printk("Stop LE scan failed (err %d)\n", err);
				continue;
			}

			/*On essaye de se connecter à ce device puisqu'il envoie une donnée avec l'UUID qu'on cherche*/
			err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN, BT_LE_CONN_PARAM_DEFAULT, &default_conn);
			if (err) {
				printk("Create connection failed (err %d)\n", err);
				/*Si erreur de connexion on recommence à scanner*/
				scan_start();
			}

			return false;
		}
	}

	return true;
}


/*--- Fonction de callback qui s'exécute lorsqu'un device est détecté pendant le scan ----*/
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
	char dev[BT_ADDR_LE_STR_LEN];

	if (rssi > -40){
		bt_addr_le_to_str(addr, dev, sizeof(dev));
		printk("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n",
			dev, type, ad->len, rssi);

	
		/* We're only interested in connectable events */
		if (type == BT_HCI_ADV_IND || type == BT_HCI_ADV_DIRECT_IND) {
			bt_data_parse(ad, eir_found, (void *)addr);
		}
	}
}


/* ------------- Fontion qui active le scan avec certains paramètres ------------------ */
static int scan_start(void)
{
	/* Use active scanning and disable duplicate filtering to handle any
	 * devices that might update their advertising data at runtime.
	 */
	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_ACTIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};

	return bt_le_scan_start(&scan_param, device_found);
}


/*------------ Fonction qui s'exécute quand l'event "bt disconnected" est reçu-------- */
static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err;

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Disconnected: %s (reason 0x%02x)\n", addr, reason);

	if (default_conn != conn) {
		return;
	}

	bt_conn_unref(default_conn);
	default_conn = NULL;

	err = scan_start();
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
	}
}


/* ------------------ Attribution des callback aux évenements -----------------*/
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};


/*------------ Fonction de callback qui s'éxecute après un read --------------*/
uint8_t read_result_callback(struct bt_conn *conn, uint8_t err,
				    struct bt_gatt_read_params *params,
				    const void *data, uint16_t length)
{
	int i;
	if (err){
		return 0;
	}
	printk("Read result : ");
	for (i = 0; i<length; i++){
		attr_value[i] = ((uint8_t*)data)[i];
		printk("%d", attr_value[i]);
	}
	printk("\n");
	return BT_GATT_ITER_STOP;
};


int main(void)
{
	/* Variable code d'erreur*/
	int err;

	/*-----------------Initialisation du bluetooth---------------*/
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}
	printk("Bluetooth initialized\n");
	/*----------------------------------------------------------*/

	/*------ Démarrage du scan pour chercher les peripheral ----*/
	err = scan_start();
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return 0;
	}
	printk("Scanning successfully started\n");
	/*----------------------------------------------------------*/
	
	while (1) {
		// Vérifier si la connexion est valide
		if (info_conn) {
			// Incrémenter la référence de la connexion pour s'assurer qu'on est connecté
			bt_conn_ref(info_conn);

			// Mise à jour des paramètres de lecture
			struct bt_uuid pt_uuid;
			/*Definition du type d'UUID*/
			pt_uuid.type = BT_UUID_TYPE_16;
			/* Fonction de callback qui s'éxecute après la lecture*/
			read_params.func = read_result_callback;
			/*Nomre de valeur qu'on va lire*/
			read_params.handle_count = 1;
			/*Handle qu'on veut lire*/
			read_params.single.handle = subscribe_params.value_handle;
			/*Offset*/
			read_params.single.offset= 0;
			/*Pointeur vers le type d'UUID qu'on a défini*/
			read_params.by_uuid.uuid = &pt_uuid;			

			/*Fonction pour lire une données du périphérique*/	
			if ((bt_gatt_read(info_conn, &read_params) == 0)){
				printk("Read succesfull\n");
			}
			else{
				printk("Read failed\n");
			}
			
			// Décrémenter la référence de la connexion
			bt_conn_unref(info_conn);
		}
		k_sleep(K_MSEC(5000));
	}
	return 0;
}
