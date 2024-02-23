#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "service.h"
#include "gpios_init.h"

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
};

struct bt_conn* connect;

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Unable to connect !\n");
		led_blink(&led0,200);
	} else {
		printk("Server connection sucess !\n");
		bt_le_adv_stop();
		connect=conn;
		led_set(&led0,HIGH);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Server disconnection\n");
	led_set(&led0,LOW);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int main(void){
	button_init(&button0);
    isr_btn_config(&button0, button0_isr, &button0_cb, GPIO_INT_EDGE_BOTH);

	led_init(&led0);
	ble_enable();
	ble_advertise_start_conn(ad, ARRAY_SIZE(ad), NULL, 0);
	while(1){
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
	ble_indicate(&button0_struct, userdata_svc, 1);
}