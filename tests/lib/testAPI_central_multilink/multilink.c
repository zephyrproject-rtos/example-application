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



////////
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

////// JORDAN
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






/////// A SUPP
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

/////// A SUPP
/*--------------------Callback parameters updated-----------------*/
static void le_param_updated(struct bt_conn *conn, uint16_t interval,
			     uint16_t latency, uint16_t timeout)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("LE conn param updated: %s int 0x%04x lat %d to %d\n",
	       addr, interval, latency, timeout);
}

// REMPLACER
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