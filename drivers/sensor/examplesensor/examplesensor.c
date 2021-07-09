/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_examplesensor

#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(examplesensor, CONFIG_SENSOR_LOG_LEVEL);

struct examplesensor_data {
	int state;
};

struct examplesensor_config {
	struct gpio_dt_spec input;
};

static int examplesensor_sample_fetch(const struct device *dev,
				      enum sensor_channel chan)
{
	const struct examplesensor_config *config = dev->config;
	struct examplesensor_data *data = dev->data;

	data->state = gpio_pin_get_dt(&config->input);

	return 0;
}

static int examplesensor_channel_get(const struct device *dev,
				     enum sensor_channel chan,
				     struct sensor_value *val)
{
	struct examplesensor_data *data = dev->data;

	if (chan != SENSOR_CHAN_PROX) {
		return -ENOTSUP;
	}

	val->val1 = data->state;

	return 0;
}

static const struct sensor_driver_api examplesensor_api = {
	.sample_fetch = &examplesensor_sample_fetch,
	.channel_get = &examplesensor_channel_get,
};

static int examplesensor_init(const struct device *dev)
{
	const struct examplesensor_config *config = dev->config;

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

	return 0;
}

#define EXAMPLESENSOR_INIT(i)						       \
	static struct examplesensor_data examplesensor_data_##i;	       \
									       \
	static const struct examplesensor_config examplesensor_config_##i = {  \
		.input = GPIO_DT_SPEC_INST_GET(i, input_gpios),		       \
	};								       \
									       \
	DEVICE_DT_INST_DEFINE(i, examplesensor_init, NULL,		       \
			      &examplesensor_data_##i,			       \
			      &examplesensor_config_##i, POST_KERNEL,	       \
			      CONFIG_SENSOR_INIT_PRIORITY, &examplesensor_api);

DT_INST_FOREACH_STATUS_OKAY(EXAMPLESENSOR_INIT)
