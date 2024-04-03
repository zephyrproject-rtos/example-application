/*
 * Copyright (c) 2021, Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef EXAMPLE_APPLICATION_INCLUDE_CUSTOM_LIB_CUSTOM_LIB_H_
#define EXAMPLE_APPLICATION_INCLUDE_CUSTOM_LIB_CUSTOM_LIB_H_

/**
 * @brief Return parameter if non-zero, or Kconfig-controlled default
 *
 * Function returns the provided value if non-zero, or a Kconfig-controlled
 * default value if the parameter is zero.  This trivial function is
 * provided in order to have a library interface example that is trivial
 * to test.
 *
 * @param return_value_if_nonzero Value to return if non-zero
 * @returns return_value_if_nonzero when the parameter is non-zero
 * @returns CONFIG_CUSTOM_LIB_GET_VALUE_DEFAULT if parameter is zero
 */
int custom_lib_get_value(int return_value_if_nonzero);

#endif /* EXAMPLE_APPLICATION_INCLUDE_CUSTOM_LIB_CUSTOM_LIB_H_ */
