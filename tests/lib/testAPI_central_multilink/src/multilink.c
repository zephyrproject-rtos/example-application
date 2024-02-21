#include "multilink.h"

/*--------------struct pour multilink---------------*/
static struct bt_conn *conn_connecting;
static uint8_t volatile conn_count;
static bool volatile is_disconnecting;

struct bt_conn* connected_devices[CONFIG_BT_MAX_CONN]; //tableau de connections
/*-------------------------------------------------*/


/*-----------------struct pour gatt-----------------*/
static struct bt_uuid_16 discover_uuid = BT_UUID_INIT_16(0);
static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_subscribe_params subscribe_params;
/*--------------------------------------------------*/


/*-----------struct pour central write--------------*/
//Write params
static struct bt_gatt_write_params write_params;
//Structure bt_conn pour récupérer les informations de connexion
static struct bt_conn *info_conn;
//Variable pour stocker les valeurs de la characteristic BT_UUID_GATT_DO du service BT_UUID_UDS
static uint8_t attr_value[] = { 0 };
//Variable pour write réussi
uint8_t write_success = 0;
/*--------------------------------------------------*/

static bool eir_found(struct bt_data *data, void *user_data)
{
	//struct paramètres bt_conn déplacé de device_found à eir_found
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
		printk("Case BT DATA UUID16 SOME\n");
	case BT_DATA_UUID16_ALL:
		printk("Case BT DATA UUID16 ALL\n");
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
				printk("résultat de comparaison : %d \n",bt_uuid_cmp(uuid, BT_UUID_UDS));
				printk("comparaison UUID non valide\n");
				continue;
			}

			/*Si c'est le même, on arrête le bt_scan*/
			err = bt_le_scan_stop();
			if (err) {
				printk("Stop LE scan failed (err %d)\n", err);
				continue;
			}

			/*On essaye de se connecter à ce device puisqu'il envoie une donnée avec l'UUID qu'on cherche*/
			err = bt_conn_le_create(addr, &create_param, &conn_param, &conn_connecting);
			if (err) {
				printk("Create connection failed (err %d)\n", err);
				ble_scan_start(NULL); //Retry scan with default parameters if failed
			}
			return false;
		}
	default :
		printk("Case abnormal BT DATA UUID16 \n");
	}

	return true;
}


static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{

	char addr_str[BT_ADDR_LE_STR_LEN];

	if (conn_connecting) {
		return;
	}

	if (rssi > -34){ //ajout filtre puissance signal

	/*Display scanned devices informations*/
	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	printk("Device found: %s (RSSI %d)\n", addr_str, rssi);

	/* We're only interested in connectable events */
	if (type == BT_HCI_ADV_IND || type == BT_HCI_ADV_DIRECT_IND) 
		{
			bt_data_parse(ad, eir_found, (void *)addr); //se connecte au device avec le callback eir_found
		return;
	}

	}
}


/*--------------Callback when received INDICATED service from connected peripheral------------*/
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

	info_conn = conn; //mis à jour d'info_conn pour répondre à la dernière connection qui a envoyé une valeur à central

	printk("Data received : ");
	for (i=0; i<length; i++){
		attr_value[i] = ((uint8_t*)data)[i];
		printk("%d ", attr_value[i]);
	}
	printk("\n");
	return BT_GATT_ITER_CONTINUE;
}


/*-------------Callback to discover received BT service and read its content------------------*/
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
		printk("attribute :%d\n", bt_gatt_attr_value_handle(attr));

		err = bt_gatt_discover(conn, &discover_params);
		if (err) {
			printk("Discover failed (err %d)\n", err);
		}
	} 
	
	else {
		/*Attribution du callback si valeur reçue par indicate*/
		subscribe_params.notify = val_received;
		subscribe_params.value = BT_GATT_CCC_INDICATE;
		subscribe_params.ccc_handle = attr->handle;

		err = bt_gatt_subscribe(conn, &subscribe_params);
		if (err && err != -EALREADY) {
			printk("Subscribe failed (err %d)\n", err);
		} else {
			printk("[SUBSCRIBED]\n");
		}

		if (conn_count < CONFIG_BT_MAX_CONN) {
	 	ble_scan_start(NULL);
	 }

		return BT_GATT_ITER_STOP;
	}



	return BT_GATT_ITER_STOP;
}



/*-----------------------Callback connected-------------------------*/
static void connected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err; //ajout variable de test erreur

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (reason) {
		printk("Failed to connect to %s (%u)\n", addr, reason);

		bt_conn_unref(conn_connecting);
		conn_connecting = NULL;

		ble_scan_start(NULL);
		return;
	}

	printk("Connected (%u): %s\n", conn_count, addr);

	/*Récupération de la connexion*/
	info_conn = conn;

	if (conn == conn_connecting) {
		connected_devices[conn_count]= conn; //ajout de l'objet conn dans le tabeau des connections

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

	conn_connecting = NULL;
	conn_count++;

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



/*----------------------Callback disconnected---------------*/
static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Disconnected: %s (reason 0x%02x)\n", addr, reason);

	bt_conn_unref(conn);

	if ((conn_count == 1U) && is_disconnecting) {
		is_disconnecting = false;
		ble_scan_start(NULL);
	}
	conn_count--;
}

/*--------------------Callback parameters request-----------------*/
static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("LE conn param req: %s int (0x%04x, 0x%04x) lat %d to %d\n",
	       addr, param->interval_min, param->interval_max, param->latency,
	       param->timeout);

	return true;
}


/*--------------------Callback parameters updated-----------------*/
static void le_param_updated(struct bt_conn *conn, uint16_t interval,
			     uint16_t latency, uint16_t timeout)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("LE conn param updated: %s int 0x%04x lat %d to %d\n",
	       addr, interval, latency, timeout);
}

/*-----------Callback register-----------*/
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







/*---------------------API FUNCTIONS-------------------------*/

int ble_enable(){
    int err;
    err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return err;
	}
	printk("Bluetooth initialized\n");

    bt_conn_cb_register(&conn_callbacks);

    return 0;

}



int ble_scan_start(bt_le_scan_cb_t cb){

    /*Scan parameter*/
	struct bt_le_scan_param scan_param = { 
		.type       = BT_LE_SCAN_TYPE_ACTIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};
	int err;

    /*Default callback*/
    if(cb==NULL)
    {
	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return err;
	}
    printk("Scanning with DEFAULT connection callback\n");
    }

    /*Custom callback*/
    else{
	err = bt_le_scan_start(&scan_param, cb);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return err;
	}
    printk("Scanning with CUSTOM connection callback\n");
    }

	return err;
}



void ble_showconn(){
    int i;
    char addr[BT_ADDR_LE_STR_LEN];

    for(i=0;i<conn_count;i++) {
        bt_addr_le_to_str(bt_conn_get_dst(connected_devices[i]), addr, sizeof(addr));
        printk("Device %d : %s\n",i,addr);
    }

}