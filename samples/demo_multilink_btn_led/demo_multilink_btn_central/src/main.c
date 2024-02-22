#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>
#include "gpios_init.h"
#include <ble_lib/ble_lib.h>

uint8_t set_led=45;
	struct Ble_Data set_led_struct={
	.size = sizeof(set_led),
	.data = &set_led,
	};

int main(void){
	button_init(&button0);
    isr_btn_config(&button0, button0_isr, &button0_cb, GPIO_INT_EDGE_RISING);
	button_init(&button1);
    isr_btn_config(&button1, button1_isr, &button1_cb, GPIO_INT_EDGE_RISING);
	button_init(&button2);
    isr_btn_config(&button2, button2_isr, &button2_cb, GPIO_INT_EDGE_RISING);

	ble_enable();
	ble_scan_start(NULL);
	// while(1){
	// 	//set_led++;
	// 	//ble_write(connected_devices[0], &set_led_struct, first_attr+2);
	// 	k_msleep(100);
	// }
	return 0;
}

void button0_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{

	set_led++;
	ble_write(connected_devices[0], &set_led_struct, first_attr+2);
}

void button1_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	set_led++;
	ble_write(connected_devices[1], &set_led_struct, first_attr+2);
}

void button2_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	ble_write(connected_devices[0], &set_led_struct, first_attr+1);
}