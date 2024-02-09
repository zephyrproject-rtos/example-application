/* main.c - Application main entry point */

/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>

#define SCAN_INTERVAL 0x0640 /* 1000 ms */
#define SCAN_WINDOW   0x0030 /* 30 ms */
#define INIT_INTERVAL 0x0010 /* 10 ms */
#define INIT_WINDOW   0x0010 /* 10 ms */
#define CONN_INTERVAL 0x0320 /* 1000 ms */
#define CONN_LATENCY  0
#define CONN_TIMEOUT  MIN(MAX((CONN_INTERVAL * 125 * \
			       MAX(CONFIG_BT_MAX_CONN, 6) / 1000), 10), 3200)

static void start_scan(void); // /!\ start_scan se nomme scan_start dans central_user_data

//struct pour multilink
static struct bt_conn *conn_connecting;
static uint8_t volatile conn_count;
static bool volatile is_disconnecting;

//struct pour gatt
static struct bt_uuid_16 discover_uuid = BT_UUID_INIT_16(0);
static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_subscribe_params subscribe_params;




/* Fonctions de central_user_data.c ajoutées manuellement*/
static uint8_t notify_func(struct bt_conn *conn,
			   struct bt_gatt_subscribe_params *params,
			   const void *data, uint16_t length)
{
	static uint8_t val_recue;

	if (!data) {
		printk("[UNSUBSCRIBED]\n");
		params->value_handle = 0U;
		return BT_GATT_ITER_STOP;
	}

	/*affichage de la valeur reçue*/
	val_recue = ((uint8_t*)data)[0];
	printf("Temperature %d.\n", val_recue);

	return BT_GATT_ITER_CONTINUE;
}



static bool eir_found(struct bt_data *data, void *user_data)
{

	//struct paramètre bt_conn déplacé de device_found à eir_found
	struct bt_conn_le_create_param create_param = {
		.options = BT_CONN_LE_OPT_NONE,
		.interval = INIT_INTERVAL,
		.window = INIT_WINDOW,
		.interval_coded = 0,
		.window_coded = 0,
		.timeout = 0,
	};
	struct bt_le_conn_param conn_param = {
		.interval_min = CONN_INTERVAL,
		.interval_max = CONN_INTERVAL,
		.latency = CONN_LATENCY,
		.timeout = CONN_TIMEOUT,
	};

	bt_addr_le_t *addr = user_data;
	int i;

	printk("[AD]: %u data_len %u\n", data->type, data->data_len);

	switch (data->type) {
	case BT_DATA_UUID16_SOME:
		printk("Cas BT DATA UUID16 SOME\n");
	case BT_DATA_UUID16_ALL:
		printk("Cas BT DATA UUID16 ALL\n");
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

			printk("début comparaison...\n");
			/*On compare si cet UUID le même que celui qu'on veut*/
			if (bt_uuid_cmp(uuid, BT_UUID_UDS)) {
				/*Si ce n'est pas le même on passe à l'itération suivante*/
				printk("résultat de comparaison : %d \n",bt_uuid_cmp(uuid, BT_UUID_UDS));
				printk("comparaison UUID non valide\n");
				continue;
			}



			//printk("comparaison fini, attempt on scan stop...\n");

			/*Si c'est le même, on arrête le bt_scan*/
			err = bt_le_scan_stop();
			//printk("scan stop attempted\n");
			if (err) {
				printk("Stop LE scan failed (err %d)\n", err);
				continue;
			}

			/*On essaye de se connecter à ce device puisqu'il envoie une donnée avec l'UUID qu'on cherche*/
			err = bt_conn_le_create(addr, &create_param, &conn_param, &conn_connecting); //on met les params pour multilink à la place des params par défaut
			//printk("connection attempted\n");
			if (err) {
				printk("Create connection failed (err %d)\n", err);
				/*Si erreur de connexion on recommence à scanner*/
				start_scan();
			}

			return false;
		}
	default :
		printk("Cas BT DATA UUID16 anormal\n");
	}

	return true;
}




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
		subscribe_params.notify = notify_func;
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





/*     fin fonctions ajoutées  	*/





static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{

	char addr_str[BT_ADDR_LE_STR_LEN];
	//int err;

	if (conn_connecting) {
		return;
	}


	if (rssi > -34){ //ajout filtre puissance signal

	//affiche infos du device trouvé
	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	printk("Device found: %s (RSSI %d)\n", addr_str, rssi);

	/* We're only interested in connectable events */
	if (type == BT_HCI_ADV_IND || type == BT_HCI_ADV_DIRECT_IND) 
		{
			bt_data_parse(ad, eir_found, (void *)addr); //se connecte au device avec le callback eir_found
		return;
	}

	}

	// /* connect only to devices in close proximity */
	// if (rssi < -70) {
	// 	return;
	// }

	// if (bt_le_scan_stop()) {
	// 	printk("Scanning successfully stopped\n");
	// 	return;
	// }

	// méthode de connection originale
	/* 	err = bt_conn_le_create(addr, &create_param, &conn_param,
				&conn_connecting);
	if (err) {
		printk("Create conn to %s failed (%d)\n", addr_str, err);
		start_scan();
	} */
}

static void start_scan(void)
{
	struct bt_le_scan_param scan_param = { //changé les params du scan pour correspondre au central_user_data
		.type       = BT_LE_SCAN_TYPE_ACTIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};
	int err;

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return;
	}

	printk("Scanning successfully started\n");
}

#if defined(CONFIG_BT_GATT_CLIENT) //structures non nécessaires, s'active lorsque CONFIG_BT_GATT_CLIENT=y dans prj.conf, on utilise pas ici
static void mtu_exchange_cb(struct bt_conn *conn, uint8_t err,
			    struct bt_gatt_exchange_params *params)
{
	printk("MTU exchange %u %s (%u)\n", bt_conn_index(conn),
	       err == 0U ? "successful" : "failed", bt_gatt_get_mtu(conn));
}

static struct bt_gatt_exchange_params mtu_exchange_params[CONFIG_BT_MAX_CONN];

static int mtu_exchange(struct bt_conn *conn)
{
	uint8_t conn_index;
	int err;

	conn_index = bt_conn_index(conn);

	printk("MTU (%u): %u\n", conn_index, bt_gatt_get_mtu(conn));

	mtu_exchange_params[conn_index].func = mtu_exchange_cb;

	err = bt_gatt_exchange_mtu(conn, &mtu_exchange_params[conn_index]);
	if (err) {
		printk("MTU exchange failed (err %d)", err);
	} else {
		printk("Exchange pending...");
	}

	return err;
}
#endif /* CONFIG_BT_GATT_CLIENT */

static void connected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err; //ajout variable de test erreur

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (reason) {
		printk("Failed to connect to %s (%u)\n", addr, reason);

		bt_conn_unref(conn_connecting);
		conn_connecting = NULL;

		start_scan();
		return;
	}

	printk("Connected (%u): %s\n", conn_count, addr);


	//printk("début du discover...\n");
	//ajout du discover
	if (conn == conn_connecting) {
		memcpy(&discover_uuid, BT_UUID_UDS, sizeof(discover_uuid));
		discover_params.uuid = &discover_uuid.uuid;
		discover_params.func = discover_func;
		discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
		discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
		discover_params.type = BT_GATT_DISCOVER_PRIMARY;

		err = bt_gatt_discover(conn_connecting, &discover_params);
		if (err) {
			printk("Discover failed(err %d)\n", err);
			return;
		}
	}
	//printk("fin du discover...\n");


	conn_connecting = NULL;

	conn_count++;
	if (conn_count < CONFIG_BT_MAX_CONN) {
		start_scan();
	}

	

#if defined(CONFIG_BT_SMP)
	int err = bt_conn_set_security(conn, BT_SECURITY_L2);

	if (err) {
		printk("Failed to set security (%d).\n", err);
	}
#endif

#if defined(CONFIG_BT_GATT_CLIENT)
	//mtu_exchange(conn); //pas besoin de mtu_exchange, activé lorsque CONFIG_BT_GATT_CLIENT=y dans prj.conf
#endif
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Disconnected: %s (reason 0x%02x)\n", addr, reason);

	bt_conn_unref(conn);

	if ((conn_count == 1U) && is_disconnecting) {
		is_disconnecting = false;
		start_scan();
	}
	conn_count--;
}

static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("LE conn param req: %s int (0x%04x, 0x%04x) lat %d to %d\n",
	       addr, param->interval_min, param->interval_max, param->latency,
	       param->timeout);

	return true;
}

static void le_param_updated(struct bt_conn *conn, uint16_t interval,
			     uint16_t latency, uint16_t timeout)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("LE conn param updated: %s int 0x%04x lat %d to %d\n",
	       addr, interval, latency, timeout);
}

#if defined(CONFIG_BT_SMP)
static void security_changed(struct bt_conn *conn, bt_security_t level,
			     enum bt_security_err err)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (!err) {
		printk("Security changed: %s level %u\n", addr, level);
	} else {
		printk("Security failed: %s level %u err %d\n", addr, level,
		       err);
	}
}
#endif

#if defined(CONFIG_BT_USER_PHY_UPDATE)
static void le_phy_updated(struct bt_conn *conn,
			   struct bt_conn_le_phy_info *param)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("LE PHY Updated: %s Tx 0x%x, Rx 0x%x\n", addr, param->tx_phy,
	       param->rx_phy);
}
#endif /* CONFIG_BT_USER_PHY_UPDATE */

#if defined(CONFIG_BT_USER_DATA_LEN_UPDATE)
static void le_data_len_updated(struct bt_conn *conn,
				struct bt_conn_le_data_len_info *info)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Data length updated: %s max tx %u (%u us) max rx %u (%u us)\n",
	       addr, info->tx_max_len, info->tx_max_time, info->rx_max_len,
	       info->rx_max_time);
}
#endif /* CONFIG_BT_USER_DATA_LEN_UPDATE */

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
	.le_param_req = le_param_req,
	.le_param_updated = le_param_updated,

#if defined(CONFIG_BT_SMP)
	.security_changed = security_changed,
#endif

#if defined(CONFIG_BT_USER_PHY_UPDATE)
	.le_phy_updated = le_phy_updated,
#endif /* CONFIG_BT_USER_PHY_UPDATE */

#if defined(CONFIG_BT_USER_DATA_LEN_UPDATE)
	.le_data_len_updated = le_data_len_updated,
#endif /* CONFIG_BT_USER_DATA_LEN_UPDATE */
};

static void disconnect(struct bt_conn *conn, void *data)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err;

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Disconnecting %s...\n", addr);
	err = bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
	if (err) {
		printk("Failed disconnection %s.\n", addr);
	}
	printk("success.\n");
}

int init_central(uint8_t iterations)
{
	int err;
	
	printk("Central_multilink begin"); //debug message

	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return err;
	}

	printk("Bluetooth initialized\n");

	bt_conn_cb_register(&conn_callbacks);

	start_scan();

	while (true) {
		while (conn_count < CONFIG_BT_MAX_CONN) {
			k_sleep(K_MSEC(10));
		}

		k_sleep(K_SECONDS(60));

		if (!iterations) {
			break;
		}
		iterations--;
		printk("Iterations remaining: %u\n", iterations);

		printk("Disconnecting all...\n");
		is_disconnecting = true;
		bt_conn_foreach(BT_CONN_TYPE_LE, disconnect, NULL);

		while (is_disconnecting) {
			k_sleep(K_MSEC(10));
		}
		printk("All disconnected.\n");
	}

	return 0;
}
