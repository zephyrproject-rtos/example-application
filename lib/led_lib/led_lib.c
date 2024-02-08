#include <led_lib/led_lib.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>



int led_init(const struct gpio_dt_spec *led) {
	if (!gpio_is_ready_dt(led)) {
		printk("\n Erreur init   leds !");
		return 0;
	}
    return 1;
}

int led_on(const struct gpio_dt_spec *led) {
    int ret = gpio_pin_configure_dt(led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
    return 1;
}

int led_off(const struct gpio_dt_spec *led) {
    int ret = gpio_pin_configure_dt(led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 0;
	}
    return 1;
}

int led_blink(const struct gpio_dt_spec *led, int time){
        led_on(led);
        k_msleep(time);
        led_off(led);
        k_msleep(time);
    return 1;
}

int led_blink_duration(const struct gpio_dt_spec *led, int time, int duree){
        int i=0;
        for(i=0;i<(duree/time);i++){
			led_blink(led,time);
        }
    return 1;      
}

int leds_init(const struct gpio_dt_spec *led){
	int i;
	int taille=sizeof(led);
	for (i=0;i<taille;i++){
		led_init(&led[i]);
	}
	return 1;
}

int leds_off(const struct gpio_dt_spec *led){
	int taille=sizeof(led);
	int i;
	for (i=0;i<taille;i++){
		led_off(&led[i]);
	}
	return 1;
}

int leds_on(const struct gpio_dt_spec *led){
	int i=0;
	int taille=sizeof(led);
	for (i=0;i<taille;i++){
		led_on(&led[i]);
	}
	return 1;
}

int leds_circular(const struct gpio_dt_spec *led, int time){
	int i=0;
	int taille=sizeof(led);
	for (i=0;i<taille;i++){
		leds_off(led);
		led_on(&led[i]);
		k_msleep(time);
	}
	return 1;
}

int leds_blink(const struct gpio_dt_spec *led, int time){
	int i=0;
	int taille=sizeof(led);
	for (i=0;i<taille;i++){
		led_on(&led[i]);
	}
	k_msleep(time);
	for (i=0;i<taille;i++){
		led_off(&led[i]);
	}
	k_msleep(time);
	return 1;
}

int leds_blink_duration(const struct gpio_dt_spec *led, int time, int duree){
	int i;
	for(i=0;i<(duree/time);i++){
		leds_blink(led,time);
    }
	return 1;
}

int led_set(const struct gpio_dt_spec *led, bool val){
	if (val) led_on(led);
	else led_off(led);
	return 1;
}