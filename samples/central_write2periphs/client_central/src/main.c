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

struct Ble_Data my_data1={
	.size = sizeof(uint32_t),
	.data = 12
};

char my_str[] = "my data 2";
struct Ble_Data my_data2={
	.size = sizeof(my_str),
	.data = &my_str
};


int main(void){
	led_init(&led0);

	ble_enable();
	ble_scan_start(NULL);
	while(1){
		ble_write(connected_devices[0],&my_data1,0);
		k_msleep(2000);
		ble_write(connected_devices[1],&my_data2,0);
		k_msleep(3000);
	}
	

	return 0;
}