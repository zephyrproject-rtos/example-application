#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h> 
#include <zephyr/drivers/sensor.h>  
#include <stdio.h>                  


#define DISPLAY_NODE DT_CHOSEN(zephyr_display)

int main(void)
{
    // --- inicialização dos Dispositivos ---
    const struct device *display_dev = DEVICE_DT_GET(DISPLAY_NODE);

    // exemplo de como você pega seu sensor (substitua pelo seu)
    // const struct device *sensor_bme = DEVICE_DT_GET(DT_NODELABEL(bme280));
    
    if (!device_is_ready(display_dev)) {
        printk("ERRO: Display device not ready.\n");
        return 0;
    }

    // if (!device_is_ready(sensor_bme)) { ... }

    display_print_clear(display_dev);
    display_print_update(display_dev);


    char display_string[48]; 

    while (1) {
        
        // --- Leitura do Sensor (COLOQUE O CÓDIGO AQUI) ---
        // sensor_sample_fetch(sensor_bme);
        // ...
        // sensor_channel_get(sensor_bme, ... &temp_value);
        
        // --- dados de exemplo para teste (SUBSTITUA PELA LEITURA REAL) ---
        double temperatura = 23.5; 
        int umidade = 45;

        // formata os dados dos sensores em uma string
        // (use \n para pular linha)
        snprintf(display_string, sizeof(display_string), "Temp: %.1f C\nUmidade: %d %%", temperatura, umidade);


        
        // limpa o que estava antes
        display_print_clear(display_dev);
        
        // escreve a nova string
        display_print(display_dev, display_string, 0); 

        // atualiza o display 
        display_print_update(display_dev);


        // atualiza a cada 2s
        k_msleep(2000); 
    }

    return 0;
}