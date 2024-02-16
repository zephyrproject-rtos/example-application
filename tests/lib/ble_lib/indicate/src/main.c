#include <ble_lib/ble_lib.h>
#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "data.h"

#define SW0_NODE	DT_ALIAS(sw0)
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button0_cb;
void button0_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#define LED1_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#define LED2_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
#define LED3_NODE DT_ALIAS(led3)
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,BT_UUID_16_ENCODE(BT_UUID_UDS_VAL)),
};


int main(void){
	strcpy(led0_cb,"t");
	button_init(&button0);
    isr_btn_config(&button0, button0_isr, &button0_cb, GPIO_INT_EDGE_BOTH);
	led_init(&led0);
	ble_enable();
	ble_advertise_start_conn(ad, ARRAY_SIZE(ad), NULL, 0);

	while(1){
		led_set(&led0,(*(uint32_t*)led0_cb_info.data));
		//printk("LED : %d\n",(*(uint32_t*)led0_cb_info.data));
		printk("LED : %s\n",(led0_cb));
		//led_set(&led1,(bool)led1_cb);
		//led_set(&led2,(bool)led2_cb);
		//led_set(&led3,(bool)led3_cb);
		k_msleep(1000);
	};

	return 0;
}

void button0_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	button0_val=0;
	if(button_state(&button0)){
		button0_val=1;
	}
	ble_indicate(&button0_struct, userdata_svc, 1);
}