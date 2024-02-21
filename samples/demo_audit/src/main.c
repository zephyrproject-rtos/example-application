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
		
	} else {
		printk("CONNECTED\n");
		bt_le_adv_stop();
		connect=conn;
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int main(void){
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
	ble_advertise_start_conn(ad, ARRAY_SIZE(ad), NULL, 0);
	//k_msleep(3000);
	while(1){
		led_set(&led0,button_led0);
		led_set(&led1,button_led1);
		led_set(&led2,button_led2);
		led_set(&led3,button_led3);
		k_msleep(1000); 
	}
	

	return 0;
}

void button0_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	button_led0=false;
	if(button_state(&button0)){
		button_led0=true;
	}
	ble_write(connect, &button_led0_struct, 1);
}

void button1_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	button_led1=false;
	if(button_state(&button1)){
		button_led1=true;
	}
	ble_write(connect, &button_led1_struct, 5);
}

void button2_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	button_led2=false;
	if(button_state(&button2)){
		button_led2=true;
	}
	ble_write(connect, &button_led2_struct, 9);
}

void button3_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	button_led3=false;
	if(button_state(&button3)){
		button_led3=true;
	}
	ble_write(connect, &button_led3_struct, 14);
}