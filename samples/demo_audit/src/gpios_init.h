#include <button_lib/button_lib.h>
#include <led_lib/led_lib.h>

#define SW0_NODE	DT_ALIAS(sw0)
static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button0_cb;
void button0_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
#define SW1_NODE	DT_ALIAS(sw1)
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET_OR(SW1_NODE, gpios, {1});
static struct gpio_callback button1_cb;
void button1_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
#define SW2_NODE	DT_ALIAS(sw2)
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET_OR(SW2_NODE, gpios, {2});
static struct gpio_callback button2_cb;
void button2_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
#define SW3_NODE	DT_ALIAS(sw3)
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET_OR(SW3_NODE, gpios, {3});
static struct gpio_callback button3_cb;
void button3_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
#define LED1_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
#define LED2_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
#define LED3_NODE DT_ALIAS(led3)
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);