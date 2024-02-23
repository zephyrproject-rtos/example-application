#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "service.h"
#include "gpios_init.h"
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

static struct bt_conn *default_conn;
static struct bt_uuid_16 discover_uuid = BT_UUID_INIT_16(0);
static struct bt_gatt_discover_params discover_params;

struct bt_conn* connect;
int first_attr;

bool etat_boutton = 1;
struct Ble_Data etat_boutton_struct={
	.data=&etat_boutton,
	.size=sizeof(etat_boutton)
};

/*--------------Fonction de callback qui s'éxecute lorsqu'une valeur est indicate -------------
------------------et qui fait l'affichage de la valeur reçue, et la stocke ------------------*/
static uint8_t val_received(struct bt_conn *conn,
			   struct bt_gatt_subscribe_params *params,
			   const void *data, uint16_t length)
{
	if (!data) {
		printk("[UNSUBSCRIBED]\n");
		params->value_handle = 0U;
		return BT_GATT_ITER_STOP;
	}

	printk("Donnée reçue : attr %d\n", params->value_handle);
	return BT_GATT_ITER_CONTINUE;
}




#define NB_CHAR 50
struct bt_gatt_subscribe_params subscribe_params_array[NB_CHAR];
int char_index = 0;





static uint8_t discover_func_test(struct bt_conn *conn,
			     const struct bt_gatt_attr *attr,
			     struct bt_gatt_discover_params *params)
{
	if (!attr) {
		if (discover_params.type == BT_GATT_DISCOVER_PRIMARY){
			bt_conn_disconnect(conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
		}
		printk("Discover complete\n");
		(void)memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	}

	int err;

	if (discover_params.type != BT_GATT_DISCOVER_CHARACTERISTIC){
		struct bt_gatt_subscribe_params *subscribe_params = &subscribe_params_array[char_index];
		subscribe_params->value_handle = (attr->handle) + 2;
		subscribe_params->notify = val_received;
		subscribe_params->value = BT_GATT_CCC_INDICATE;
		subscribe_params->ccc_handle = (attr->handle) + 3;

		err = bt_gatt_subscribe(conn, subscribe_params);
		if (err && err != -EALREADY) {
			printk("Subscribe failed (err %d)\n", err);
		} 
		else {
			printk("[SUBSCRIBED]\n");
		}

		discover_params.uuid = NULL;
		discover_params.start_handle = (attr->handle) + 4;
		discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;
		discover_params.func = discover_func_test;
		discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;

		err = bt_gatt_discover(default_conn, &discover_params);
		if (err) {
			printk("Discover failed(err %d)\n", err);
			return;
		}
	}
	else{
		struct bt_gatt_subscribe_params *subscribe_params = &subscribe_params_array[char_index];
		subscribe_params->value_handle = (attr->handle) + 1;
		subscribe_params->notify = val_received;
		subscribe_params->value = BT_GATT_CCC_INDICATE;
		subscribe_params->ccc_handle = (attr->handle) + 2;

		err = bt_gatt_subscribe(conn, subscribe_params);
		if (err && err != -EALREADY) {
			printk("Subscribe failed (err %d)\n", err);
		} 
		else {
			printk("[SUBSCRIBED]\n");
		}

		discover_params.uuid = NULL;
		discover_params.start_handle = (attr->handle) + 1;
		discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;
		discover_params.func = discover_func_test;
		discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;

		err = bt_gatt_discover(default_conn, &discover_params);
		if (err) {
			printk("Discover failed(err %d)\n", err);
			return;
		}
		
	}
	char_index++;
	return BT_GATT_ITER_STOP;
}


void discover_gatt_service(uint16_t service_UUID)
{
	int err;
	memcpy(&discover_uuid, BT_UUID_DECLARE_16(service_UUID), sizeof(discover_uuid));
	discover_params.uuid = &discover_uuid.uuid;
	discover_params.func = discover_func_test;
	discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
	discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
	discover_params.type = BT_GATT_DISCOVER_PRIMARY;

	err = bt_gatt_discover(default_conn, &discover_params);
	if (err) {
		printk("Discover failed(err %d)\n", err);
		return;
	}
}




static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
	} 
	else{
		printk("connetced\n;");
		connect=conn;
		discover_gatt_service(BT_UUID_UDS_VAL);
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
				//scan_start();
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

	/* Si le signal n'est pas assez fort (appareil trop loin), on passe */
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



static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected\n");
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};








int main(void){
	int err;
	printk("NULL :%d\n", NULL==default_conn );
	button_init(&button0);
    isr_btn_config(&button0, button0_isr, &button0_cb, GPIO_INT_EDGE_BOTH);
	button_init(&button1);
    isr_btn_config(&button1, button1_isr, &button1_cb, GPIO_INT_EDGE_BOTH);
	button_init(&button2);
    isr_btn_config(&button2, button2_isr, &button2_cb, GPIO_INT_EDGE_BOTH);
	button_init(&button3);
    isr_btn_config(&button3, button3_isr, &button3_cb, GPIO_INT_EDGE_BOTH);

	led_init(&led0);
	led_init(&led1);
	led_init(&led2);
	led_init(&led3);

	ble_enable();
	/*------ Démarrage du scan pour chercher les peripheral ----*/
	err = scan_start();
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return 0;
	}
	printk("Scanning successfully started\n");
	/*----------------------------------------------------------*/
	k_msleep(3000);
	while(1){
		led_set(&led0,button_led0);
		led_set(&led1,button_led1);
		led_set(&led2,button_led2);
		led_set(&led3,button_led3);
		k_msleep(1);
	}
	

	return 0;
}

void button0_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	ble_write(connect, &etat_boutton_struct, first_attr);

}

void button1_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	ble_write(connect, &etat_boutton_struct, first_attr+4);
}

void button2_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	ble_write(connect, &etat_boutton_struct, first_attr+8);
}

void button3_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	ble_write(connect, &etat_boutton_struct, first_attr+12);
}