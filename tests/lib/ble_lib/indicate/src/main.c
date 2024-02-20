#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "service.h"
#include "gpios_init.h"


struct bt_uuid_16 discover_uuid = BT_UUID_INIT_16(0);
struct bt_gatt_discover_params discover_params;
struct bt_gatt_subscribe_params subscribe_params;


static const struct bt_data ad[] = {
	//BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
};

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
		printk("%d ", *((uint8_t*)data));
	}
	printk("\n");
	return BT_GATT_ITER_CONTINUE;
}

uint8_t discover_func(struct bt_conn *conn,
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

static struct bt_conn *connect;

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		led_blink(&led0, 100);
		
	} else {
		led_set(&led0, HIGH);
		connect=conn;
		bt_le_adv_stop();

		memcpy(&discover_uuid, BT_UUID_UDS, sizeof(discover_uuid));
		discover_params.uuid = &discover_uuid.uuid;
		discover_params.func = discover_func;
		discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
		discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
		discover_params.type = BT_GATT_DISCOVER_PRIMARY;
		int err = bt_gatt_discover(connect, &discover_params);
	}
}




static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	led_set(&led0, LOW);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int main(void){
	uint8_t val=15;
	struct Ble_Data val_info={
		.data=&val,
		.size=sizeof(val)
	};
	button_init(&button0);
    isr_btn_config(&button0, button0_isr, &button0_cb, GPIO_INT_EDGE_BOTH);
	led_init(&led0);
	ble_enable();
	ble_advertise_start_conn(ad, ARRAY_SIZE(ad), NULL, 0);

		


	
	k_msleep(3000);
	while(1){
		//ble_write(connect, &val_info, subscribe_params.value_handle);
		k_msleep(100);
	}
	

	return 0;
}

void button0_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	button0_val=false;
	if(button_state(&button0)){
		button0_val=true;
	}
	ble_indicate(&button0_struct, gpios_svc, 1);
}

/*NOTE : interet du indicate*/

/*
Le indicate permet de signaler des données TR
EN effet le indicate est utile pour des boutons par exemple, sur interruption
la centrale n'a pas a constamment interroger le service bouton pour connaitre son état
*/