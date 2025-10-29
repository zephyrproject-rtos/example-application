#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/**
 * @brief Ponto de entrada principal da aplicação.
 *
 * Esta função é executada quando o firmware inicia na placa.
 * Ela NÃO é executada quando rodamos os testes com `west test`.
 */
int main(void)
{
    printk("Aplicação do Sensor de Temperatura Iniciada.\n");
    printk("Aguardando integração com o hardware...\n");

    // LÓGICA FUTURA:
    // 1. Obter a referência para o dispositivo sensor a partir do Device Tree.
    // 2. Inicializar o driver do sensor.
    // 3. Em um loop, chamar a função de leitura de temperatura do driver.
    // 4. Imprimir a temperatura lida a cada segundo.
}