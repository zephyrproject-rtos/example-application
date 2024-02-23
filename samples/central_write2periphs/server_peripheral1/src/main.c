#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "service.h"



static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
};

struct bt_conn* connect;

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Unable to connect !\n");

	} else {
		printk("Server connection sucess !\n");
		bt_le_adv_stop();
		connect=conn;

	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Server disconnection\n");

}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};





int main(void){

	ble_enable();
	ble_advertise_start_conn(ad, ARRAY_SIZE(ad), NULL, 0);
	while(1){
		printk("Data received : %d", (uint8_t) mydata_received.data);
		k_msleep(100); 
	}
	

	return 0;
}

