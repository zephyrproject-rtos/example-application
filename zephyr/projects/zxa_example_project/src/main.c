/*
 * Copyright (c) 2021 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(zxa);

void main(void)
{
	LOG_INF("Hello from " APP_PROJECT_NAME "!\n");
}
