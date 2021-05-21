/*
 * Copyright (c) 2021, Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <custom_lib/custom_lib.h>

int custom_lib_get_value(int return_value_if_nonzero)
{
	return (return_value_if_nonzero != 0) ? return_value_if_nonzero
		: CONFIG_CUSTOM_LIB_GET_VALUE_DEFAULT;
}
