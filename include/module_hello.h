/*
 * Copyright (c) 2021 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/** @file
 *  @brief Module Hello API
 */
#ifndef EXAMPLE_APPLICATION_INCLUDE_MODULE_HELLO_H_
#define EXAMPLE_APPLICATION_INCLUDE_MODULE_HELLO_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup isr_apis Interrupt Service Routine APIs
 * @ingroup kernel_apis
 * @{
 */

/**
 * @brief Return the number of times module_hello said this power-up.
 *
 * @retval >= 0 If successful.
 * @retval Errno Negative errno code if failure.
 */
int32_t module_hello_said(void);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* EXAMPLE_APPLICATION_INCLUDE_MODULE_HELLO_H_ */
