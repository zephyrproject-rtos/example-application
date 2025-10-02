/*
 * Semana 1 – HAL (Zephyr) – app mínimo com duas threads
 * Thread A: pisca LED (se alias led0 existir) ou só loga
 * Thread B: "heartbeat" no console
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(hal_week1, LOG_LEVEL_INF);

/* Tenta usar o alias led0, se a placa tiver definido no Devicetree */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static bool led_is_ready(void)
{
    return device_is_ready(led0.port) == true;
}
#else
#warning "Alias 'led0' não encontrado no Devicetree desta placa."
static bool led_is_ready(void) { return false; }
#endif

/* Configurações das threads */
#define STACK_SZ 1024
#define PRIO_BLINK 5
#define PRIO_HEART 6

K_THREAD_STACK_DEFINE(stack_blink, STACK_SZ);
K_THREAD_STACK_DEFINE(stack_heart, STACK_SZ);
static struct k_thread thread_blink_data;
static struct k_thread thread_heart_data;

static void blink_thread(void *a, void *b, void *c)
{
    ARG_UNUSED(a); ARG_UNUSED(b); ARG_UNUSED(c);

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
    if (!led_is_ready()) {
        LOG_WRN("GPIO do LED não está pronto; entrando em modo somente log.");
    } else {
        int ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
        if (ret) {
            LOG_ERR("Falha ao configurar LED (ret=%d); usando somente log.", ret);
        }
    }
#endif

    while (1) {
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
        if (led_is_ready()) {
            gpio_pin_toggle_dt(&led0);
        }
#endif
        LOG_INF("[BLINK] toggle");
        k_msleep(500);
    }
}

static void heartbeat_thread(void *a, void *b, void *c)
{
    ARG_UNUSED(a); ARG_UNUSED(b); ARG_UNUSED(c);
    uint32_t cnt = 0;
    while (1) {
        LOG_INF("[HEART] alive %u", cnt++);
        k_msleep(1000);
    }
}

int main(void)
{
    LOG_INF("HAL Week1 app start");

    /* Cria as duas threads com prioridades diferentes */
    k_tid_t t1 = k_thread_create(&thread_blink_data, stack_blink, STACK_SZ,
                                 blink_thread, NULL, NULL, NULL,
                                 PRIO_BLINK, 0, K_NO_WAIT);

    k_tid_t t2 = k_thread_create(&thread_heart_data, stack_heart, STACK_SZ,
                                 heartbeat_thread, NULL, NULL, NULL,
                                 PRIO_HEART, 0, K_NO_WAIT);

    /* Opcional: nomear threads para facilitar debug */
    k_thread_name_set(t1, "blink");
    k_thread_name_set(t2, "heart");

    /* A main pode ficar ociosa ou realizar alguma supervisão */
    while (1) {
        k_msleep(2000);
    }
    return 0;
}
