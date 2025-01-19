/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_example_sensor

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(example_sensor, CONFIG_SENSOR_LOG_LEVEL);

//static FILE *csv_file = NULL;

struct example_sensor_data {
    int state;
    FILE *csv_file;
};

struct example_sensor_config {
    struct gpio_dt_spec input;
};

static int example_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    const struct example_sensor_config *config = dev->config;
    struct example_sensor_data *data = dev->data;

    /*
    Toma la data del sensor...
    */
    // data->state = gpio_pin_get_dt(&config->input);

    // csv_file = fopen(CONFIG_PATH_TO_CSV_FILE, "r");

    if(!data->csv_file){
        data->csv_file = fopen(CONFIG_PATH_TO_CSV_FILE, "r");
        if(!data->csv_file){
            LOG_ERR("Error opening file");
            return -ENOENT;
        }
    }

    char line[CONFIG_SIZE_LINE_MAX];

    if (fgets(line, sizeof(line), data->csv_file))
    {
        int value;
        if (sscanf(line, "%d", &value) == 1)
        {
            data->state = value;
        }
        else
        {
            LOG_ERR("Error reading file");
            return -EIO;
        }
    }
    else
    {
        LOG_INF("Reading file last number");
        fclose(data->csv_file);
        data->csv_file = NULL;
    }

    return 0;
}

static int example_sensor_channel_get(const struct device * dev,
                                      enum sensor_channel   chan,
                                      struct sensor_value * val)
{
    struct example_sensor_data *data = dev->data;

    if (chan != SENSOR_CHAN_PROX && chan !=  SENSOR_CHAN_AMBIENT_TEMP) {
        return -ENOTSUP;
    }

    val->val1 = data->state;

// ESTO ES PARA PUENTEAR EL SENSOR A UN VALOR 100 FIJO
//    val->val1 = 100;

    return 0;
}

static DEVICE_API(sensor, example_sensor_api) = {
    .sample_fetch = &example_sensor_sample_fetch,
    .channel_get = &example_sensor_channel_get,
};

static int example_sensor_init(const struct device *dev)
    {

    const struct example_sensor_config *config = dev->config;
    struct example_sensor_data *data = dev->data;
    int ret;

    if (!device_is_ready(config->input.port)) {
        LOG_ERR("Input GPIO not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&config->input, GPIO_INPUT);

    if (ret < 0) {
        LOG_ERR("Could not configure input GPIO (%d)", ret);
        return ret;
    }

    data->csv_file = NULL;
    return 0;
}

#define EXAMPLE_SENSOR_INIT(i)						       					\
    static struct example_sensor_data example_sensor_data_##i;	       		\
                                                                            \
    static const struct example_sensor_config example_sensor_config_##i = {	\
        .input = GPIO_DT_SPEC_INST_GET(i, input_gpios),		       			\
    };								       									\
                                                                            \
    DEVICE_DT_INST_DEFINE(i, example_sensor_init, NULL,		       			\
                  &example_sensor_data_##i,			       					\
                  &example_sensor_config_##i, POST_KERNEL,	                \
                  CONFIG_SENSOR_INIT_PRIORITY, &example_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(EXAMPLE_SENSOR_INIT)
