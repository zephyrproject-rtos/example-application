/*
 * Copyright (c) 2021, Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <app/lib/custom.h>

int custom_get_value(int val)
{
	return (val != 0) ? val : CONFIG_CUSTOM_GET_VALUE_DEFAULT;
}
