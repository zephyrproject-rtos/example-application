/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdio.h>
#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void)
{
	const struct device *sensor;
	int ret;
	struct sensor_value value_x;

	sensor = DEVICE_DT_GET(DT_NODELABEL(example_sensor));

	if (!device_is_ready(sensor)) {
		LOG_ERR("Sensor not ready");
		return 0;
	}

	LOG_INF("Zephyr Example Application %s\n", APP_VERSION_STRING);

	init_my_timer();
	start_my_timer();

	while (1) {
        k_sleep(K_MSEC(2000));
        LOG_INF("loop...");

		ret = sensor_sample_fetch(sensor);
		if (ret) {
			printk("sensor_sample_fetch failed ret %d\n", ret);
			return 0;
		}

		ret = sensor_channel_get(sensor, SENSOR_CHAN_PROX, &value_x);
		printk("sensor_sample_get ret: %d  valor del sensor: %d\n", ret, value_x.val1);

    }

	return 0;
}