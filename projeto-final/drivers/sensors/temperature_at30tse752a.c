/*
 * Copyright (c) 2024 UFSM
 * Driver para sensor de temperatura AT30TSE752A
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

struct temperature_data {
    int16_t temperature;
};

struct temperature_config {
    struct i2c_dt_spec i2c;
};

static int temperature_init(const struct device *dev)
{
    const struct temperature_config *config = dev->config;
    
    if (!device_is_ready(config->i2c.bus)) {
        LOG_ERR("Barramento I2C não está pronto");
        return -ENODEV;
    }
    
    LOG_INF("Sensor AT30TSE752A inicializado");
    return 0;
}

static int temperature_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    LOG_INF("sample_fetch: Será implementado na semana 4 - Leitura I2C");
    return -ENOTSUP;
}

static int temperature_channel_get(const struct device *dev, enum sensor_channel chan,
                                  struct sensor_value *val)
{
    LOG_INF("channel_get: Será implementado na semana 5 - Conversão temperatura");
    return -ENOTSUP;
}

static const struct sensor_driver_api temperature_api = {
    .sample_fetch = temperature_sample_fetch,
    .channel_get = temperature_channel_get,
};

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
