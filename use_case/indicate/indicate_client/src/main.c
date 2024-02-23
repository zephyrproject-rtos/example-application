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
	while(1){
		//led_set(&led0,*((bool*)read_value));
		k_msleep(1000);
	}
	

	return 0;
}