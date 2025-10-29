#include <zephyr/kernel.h>
#include <zephyr/device.h>       // Necessário para a API de dispositivos
#include <zephyr/drivers/sensor.h> // Necessário para a API de sensores
#include <zephyr/logging/log.h>
#include <stdio.h>               // Necessário para snprintf (para imprimir floats)

// (Opcional, mas útil) Inclui seu driver para a função de conversão,
// embora não a usemos aqui, já que a API do sensor já converte para nós.
// #include "driver_temp.h" 

LOG_MODULE_REGISTER(app_main, CONFIG_LOG_DEFAULT_LEVEL);



//    nome padrao do sensor na placa de shield I/O1 Xplained Pro
#define SENSOR_NODE DT_NODELABEL(at30tse)

// 2. Verifique se o no existe no Device Tree
#if !DT_NODE_HAS_STATUS(SENSOR_NODE, okay)
    #error "Erro: No do sensor 'at30tse' nao encontrado ou desabilitado."
#endif

// 3. Obtenha o dispositivo (struct device) a partir do no

static const struct device *sensor_dev = DEVICE_DT_GET(SENSOR_NODE);


int main(void)
{
    struct sensor_value temp_val;
    double temp_celsius;

    printk("Aplicacao do Sensor de Temperatura (Hardware Real) iniciada.\n");

    // 4. Verifica se o driver do sensor esta pronto para ser usado
    if (!device_is_ready(sensor_dev)) {
        printk("Erro: Sensor real '%s' nao esta pronto.\n", sensor_dev->name);
        return -1; // Sai se o sensor nao for encontrado
    }

    printk("Lendo do sensor real: %s\n", sensor_dev->name);

    while (1) {
        // 5. Busca uma nova "leitura" do sensor
        //    (Isso ira se comunicar via I2C com o chip AT30TSE)
        if (sensor_sample_fetch(sensor_dev) != 0) {
            printk("Erro: Falha ao buscar leitura do sensor real (I2C?).\n");
            k_sleep(K_SECONDS(10));
            continue;
        }

        // 6. Pega o canal de "Temperatura Ambiente" do sensor
        if (sensor_channel_get(sensor_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_val) != 0) {
            printk("Erro: Falha ao ler o canal de temperatura do sensor.\n");
            k_sleep(K_SECONDS(10));
            continue;
        }

        // 7. Converte o valor do sensor (struct) para um double (float)
        //    (O driver do AT30TSE faz a conversao para Celsius automaticamente)
        temp_celsius = sensor_value_to_double(&temp_val);

        // 8. Imprime o valor!
        //    Usamos snprintf porque o printk padrao do Zephyr nao
        //    lida com floats (%f) a menos que habilitado (o que gasta muita memoria).
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.2f", temp_celsius);
        printk("Temperatura ATUAL: %s C\n", buffer);


        // Aguarda 10 segundos antes de ler novamente
        k_sleep(K_SECONDS(10));
    }
    return 0;
}