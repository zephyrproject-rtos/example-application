#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h> // API de Controle de Pinos (para o LED)

// Tempo de espera entre as piscadas (1000ms = 1 segundo)
#define SLEEP_TIME_MS   1000

/*
 * O Zephyr encontra o LED0 automaticamente no "mapa" (Device Tree)
 * da placa samr21_xpro.
 */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
    printk("Iniciando o Blinky (Pisca-LED) na SAM R21!\n");

    // 1. Verifica se o hardware do LED está pronto
    if (!device_is_ready(led.port)) {
        printk("Erro: dispositivo LED %s não está pronto.\n", led.port->name);
        return -1;
    }

    // 2. Configura o pino do LED como Saída (OUTPUT)
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) != 0) {
        printk("Erro: falha ao configurar o LED.\n");
        return -1;
    }

    printk("LED configurado. Iniciando loop de piscar...\n");

    // 3. Loop infinito
    while (1) {
        // Liga/Desliga o LED
        gpio_pin_toggle_dt(&led);
        printk("Estado do LED alterado!\n");

        // Aguarda 1 segundo
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
    return 0;
}