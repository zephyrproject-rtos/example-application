/*
 * Copyright (c) 2021 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <init.h>
#include <module_hello.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(module_hello);

int32_t module_hello_said(void)
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
