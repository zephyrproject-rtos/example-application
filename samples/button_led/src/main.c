#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>

#define SW0_NODE	DT_ALIAS(sw0)
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main() {
    button_init(&button0);
    led_init(&led0);
    while (1) {
        if(button_state(&button0)) led_on(&led0);
        else led_off(&led0);
    }
    return 0;
}
