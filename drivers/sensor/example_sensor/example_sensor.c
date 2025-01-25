/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_example_sensor

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(example_sensor, CONFIG_SENSOR_LOG_LEVEL);

// timer expires
// Declare the timer
struct k_timer my_timer;

// Timer expiry function
void my_timer_expiry_function(struct k_timer *timer_id) {
    LOG_INF("Timer expired");
}

// Timer stop function - el expires timer doesn't have stop function...
void my_timer_stop_function(struct k_timer *timer_id) {
    LOG_INF("Timer stopped");
}

// Initialize the timer
void init_my_timer(void) {
    k_timer_init(&my_timer, my_timer_expiry_function, my_timer_stop_function);
    LOG_INF("Timer Initialized");
}

// In your function where you start the timer
// start a periodic timer that expires once every CONFIG_MY_TIMER_PERIOD
void start_my_timer(void) {
    k_timer_start(&my_timer, K_SECONDS(CONFIG_MY_TIMER_INITIAL_DELAY), K_SECONDS(CONFIG_MY_TIMER_PERIOD));
}

K_TIMER_DEFINE(my_timer, my_timer_expiry_function, NULL);

//static FILE *csv_file = NULL;
struct example_sensor_data {
    int state;
    FILE *csv_file;
};

struct example_sensor_config {

};

static int example_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    struct example_sensor_data *data = dev->data;

    /*
    Toma la data del sensor...
    */

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
                                      struct sensor_value * val){
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
    struct example_sensor_data *data = dev->data;
    data->csv_file = NULL;
    return 0;
}

#define EXAMPLE_SENSOR_INIT(i)						       					\
    static struct example_sensor_data example_sensor_data_##i;	       		\
                                                                            \
    static const struct example_sensor_config example_sensor_config_##i = {	\
    };								       									\
                                                                            \
    DEVICE_DT_INST_DEFINE(i, example_sensor_init, NULL,		       			\
                  &example_sensor_data_##i,			       					\
                  &example_sensor_config_##i, POST_KERNEL,	                \
                  CONFIG_SENSOR_INIT_PRIORITY, &example_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(EXAMPLE_SENSOR_INIT)
