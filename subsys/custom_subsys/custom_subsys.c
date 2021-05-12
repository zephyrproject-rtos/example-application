/*
 * Copyright (c) 2021 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <example-application/custom_subsys/custom_subsys.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(custom_subsys);

int32_t custom_subsys_said(void)
{
	return 1;
}

static int init(const struct device *dev)
{
	ARG_UNUSED(dev);

	LOG_INF("Hello from " STRINGIFY(MODULE_NAME));

	return 0;
}

SYS_INIT(init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
