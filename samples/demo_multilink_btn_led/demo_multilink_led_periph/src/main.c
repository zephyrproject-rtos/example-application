#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "service.h"
#include "gpios_init.h"

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
};


static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		
	} else {
		printk("CONNECTED\n");
		led_set(&led3,HIGH);
		bt_le_adv_stop();
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	led_set(&led3,LOW);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int main(void){
	led_init(&led0);

	ble_enable();
	ble_advertise_start_conn(ad, ARRAY_SIZE(ad), NULL, 0);
	k_msleep(3000);
	while(1){
		printk("Led : %d\n",led0_val);
		led_set(&led0,led0_val);
		k_msleep(100); 
	}
	return 0;
}