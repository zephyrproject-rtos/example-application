/*
 * Copyright (c) 2024 UFSM
 * Driver para sensor de temperatura AT30TSE752A
 * Semana 4: Implementação I2C - Build sobre Semana 3
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(temp_at30tse752a, CONFIG_SENSOR_LOG_LEVEL);

/* Endereço I2C do sensor */
#define AT30TSE752A_ADDR          0x49

/* Registradores do sensor */
#define AT30TSE752A_REG_TEMP      0x00
#define AT30TSE752A_REG_CONFIG    0x01
#define AT30TSE752A_REG_EEPROM    0x05

/* Valores de configuração */
#define AT30TSE752A_CONFIG_DEFAULT 0x00  // 12-bit resolution, continuous conversion

// ✅ MANTIDO da Semana 3
struct temperature_data {
    int16_t temperature;  // Já existente - vamos usar para dados brutos
    bool device_ready;    // ✅ NOVO: adicionar flag de status
};

// ✅ MANTIDO da Semana 3  
struct temperature_config {
    struct i2c_dt_spec i2c;  // Já existente
};

/* ✅ NOVO: Função para verificar se o sensor está presente */
static int at30tse752a_probe(const struct device *dev)
{
    const struct temperature_config *config = dev->config;
    uint8_t config_reg;
    int ret;

    /* Tentar ler o registrador de configuração */
    ret = i2c_reg_read_byte_dt(&config->i2c, AT30TSE752A_REG_CONFIG, &config_reg);
    if (ret < 0) {
        LOG_ERR("Falha na comunicação I2C com sensor (código: %d)", ret);
        return ret;
    }

    LOG_INF("Sensor AT30TSE752A detectado! Config: 0x%02X", config_reg);
    return 0;
}

/* ✅ NOVO: Função para configurar o sensor */
static int at30tse752a_config_set(const struct device *dev)
{
    const struct temperature_config *config = dev->config;
    int ret;

    /* Configurar sensor para modo padrão (12-bit) */
    ret = i2c_reg_write_byte_dt(&config->i2c, AT30TSE752A_REG_CONFIG, AT30TSE752A_CONFIG_DEFAULT);
    if (ret < 0) {
        LOG_ERR("Falha ao configurar sensor: %d", ret);
        return ret;
    }

    LOG_INF("Sensor AT30TSE752A configurado com sucesso");
    return 0;
}

// ✅ MODIFICADO: Expandir a init existente da Semana 3
static int temperature_init(const struct device *dev)
{
    const struct temperature_config *config = dev->config;
    struct temperature_data *data = dev->data;
    int ret;

    // ✅ MANTIDO da Semana 3
    if (!device_is_ready(config->i2c.bus)) {
        LOG_ERR("Barramento I2C não está pronto");
        return -ENODEV;
    }
    
    // ✅ NOVO: Verificar se sensor está presente
    ret = at30tse752a_probe(dev);
    if (ret < 0) {
        return ret;
    }

    // ✅ NOVO: Configurar sensor
    ret = at30tse752a_config_set(dev);
    if (ret < 0) {
        return ret;
    }

    // ✅ NOVO: Marcar dispositivo como pronto
    data->device_ready = true;
    
    LOG_INF("Sensor AT30TSE752A inicializado com sucesso");
    return 0;
}

// ✅ MODIFICADO: Implementar a função placeholder da Semana 3
static int temperature_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    const struct temperature_config *config = dev->config;
    struct temperature_data *data = dev->data;
    uint8_t temp_data[2];
    int ret;

    // ✅ NOVO: Verificar se dispositivo está pronto
    if (!data->device_ready) {
        LOG_ERR("Dispositivo não está pronto");
        return -ENODEV;
    }

    if (chan != SENSOR_CHAN_ALL && chan != SENSOR_CHAN_AMBIENT_TEMP) {
        return -ENOTSUP;
    }

    // ✅ NOVO: Ler 2 bytes do registrador de temperatura
    ret = i2c_burst_read_dt(&config->i2c, AT30TSE752A_REG_TEMP, temp_data, 2);
    if (ret < 0) {
        LOG_ERR("Falha na leitura de temperatura: %d", ret);
        return ret;
    }

    // ✅ NOVO: Combinar os 2 bytes (big-endian)
    data->temperature = (temp_data[0] << 8) | temp_data[1];

    LOG_DBG("Dados brutos: 0x%02X%02X = %d", temp_data[0], temp_data[1], data->temperature);
    LOG_INF("Leitura I2C bem-sucedida - Valor bruto: %d", data->temperature);

    return 0;
}

// ✅ MANTIDO como placeholder para Semana 5 (igual da Semana 3)
static int temperature_channel_get(const struct device *dev, enum sensor_channel chan,
                                  struct sensor_value *val)
{
    struct temperature_data *data = dev->data;

    if (chan != SENSOR_CHAN_AMBIENT_TEMP) {
        return -ENOTSUP;
    }

    /* ✅ MANTIDO da Semana 3 - Placeholder para conversão real na Semana 5 */
    LOG_INF("channel_get: Será implementado na semana 5 - Conversão temperatura");
    return -ENOTSUP;
}

// ✅ MANTIDO da Semana 3
static const struct sensor_driver_api temperature_api = {
    .sample_fetch = temperature_sample_fetch,
    .channel_get = temperature_channel_get,
};

// ✅ MANTIDO da Semana 3
#define TEMPERATURE_INIT(n) \
    static struct temperature_data temperature_data_##n; \
    static const struct temperature_config temperature_config_##n = { \
        .i2c = I2C_DT_SPEC_INST_GET(n), \
    }; \
    DEVICE_DT_INST_DEFINE(n, temperature_init, NULL, \
                         &temperature_data_##n, &temperature_config_##n, \
                         POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY, \
                         &temperature_api);

DT_INST_FOREACH_STATUS_OKAY(TEMPERATURE_INIT)
