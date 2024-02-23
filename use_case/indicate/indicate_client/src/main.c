#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "gpios_init.h"



static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		
	} else {
		printk("CONNECTED\n");
		ble_discover_gatt_service(BT_UUID_UDS_VAL);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("DISCONNECTED\n");
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int main(void){
	led_init(&led0);

	ble_enable();
	ble_scan_start(NULL);

	uint8_t valeur = 5;
	struct Ble_Data data_send = {
		.data = &valeur,
		.size = sizeof(valeur),
	};

	uint8_t valeur3 = 0;
	struct Ble_Data data_send13 = {
		.data = &valeur3,
		.size = sizeof(valeur3),
	};

	k_msleep(15000);

	while(1){
		//led_set(&led0,*((bool*)read_value));
		k_msleep(1000);
	
		valeur = 5;
		ble_write(connected_devices[0], &data_send, 0);
		k_msleep(1000);
		ble_read(connected_devices[0], &data_send13, 0);
		printk("Valeur : %d\n", *((uint8_t*)data_send13.data));

		valeur = 68;
		ble_write(connected_devices[0], &data_send, 1);
		k_msleep(1000);
		ble_read(connected_devices[0], &data_send13, 1);
		printk("Valeur : %d\n", valeur3);

	}
	
	

	return 0;
}