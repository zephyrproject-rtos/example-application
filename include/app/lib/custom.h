/*
 * Copyright (c) 2021, Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_LIB_CUSTOM_H_
#define APP_LIB_CUSTOM_H_

/**
 * @defgroup lib_custom Custom library
 * @ingroup lib
 * @{
 *
 * @brief An example of a custom out-of-tree library.
 *
 * This library illustrates how create custom out-of-tree libraries. Splitting
 * code in libraries enables code reuse and modularity, also easing testing.
 */

/**
 * @brief Return @p val if non-zero, or Kconfig-controlled default.
 *
 * Function returns the provided value if non-zero, or a Kconfig-controlled
 * default value if the parameter is zero. This trivial function is provided in
 * order to have a library interface example that is trivial to test.
 *
 * @param val Value to return if non-zero
 *
 * @retval val if @p val is non-zero
 * @retval CONFIG_CUSTOM_GET_VALUE_DEFAULT if @p val is zero
 */
int custom_get_value(int val);

/** @} */

#endif /* APP_LIB_CUSTOM_H_ */
