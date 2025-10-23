#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdlib.h>

#define SLEEP_TIME_MS 2000

/* Simulação de sensores */
static int ler_temperatura(void)
{
    return 20 + rand() % 10; // Simula 20°C a 29°C
}

static int ler_luminosidade(void)
{
    return rand() % 1000; // Simula 0 a 1000 lux
}

void main(void)
{
    printk("=== Mini Estação Ambiental Thread - UFSM ===\n");

    while (1) {
        int temperatura = ler_temperatura();
        int luz = ler_luminosidade();

        printk("Temperatura: %d °C | Luminosidade: %d lx\n",
               temperatura, luz);

        k_msleep(SLEEP_TIME_MS);
    }
}
