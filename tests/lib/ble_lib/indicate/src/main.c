#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "service.h"
#include "gpios_init.h"

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		led_blink(&led0, 100);
	} else {
		led_set(&led0, HIGH);
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
	button_init(&button0);
    isr_btn_config(&button0, button0_isr, &button0_cb, GPIO_INT_EDGE_BOTH);
	led_init(&led0);
	ble_enable();
	ble_advertise_start_conn(ad, ARRAY_SIZE(ad), NULL, 0);

	while(1){
		k_msleep(1000);
	};

	return 0;
}

void button0_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	strcpy(button0_val,"xyz");//Test de chaine de caractere
	if(button_state(&button0)){
		strcpy(button0_val,"abcdef");
	}
	ble_indicate(&button0_struct, gpios_svc, 1);
}

/*NOTE : interet du indicate*/

/*
Le indicate permet de signal des données TR
EN effet le indicate est utile pour des boutons par exemple, sur interruption
la centrale n'a pas a constamment interroger le service bouton pour connaitre son état
*/