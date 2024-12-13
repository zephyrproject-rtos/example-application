/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main(void)
{
	LOG_INF("Zephyr Example Application %s\n", APP_VERSION_STRING);

	while(1){
		k_sleep(K_MSEC(2000));
		LOG_INF("loop...");
	}

	return 0;
}