/*
 * Copyright (c) 2021 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/** @file
 *  @brief Example-application's Custom Subsys API
 */
#ifndef EXAMPLE_APPLICATION_INCLUDE_EXAMPLE_APPLICATION_CUSTOM_SUBSYS_CUSTOM_SUBSYS_H_
#define EXAMPLE_APPLICATION_INCLUDE_EXAMPLE_APPLICATION_CUSTOM_SUBSYS_CUSTOM_SUBSYS_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup example_apis Example Application APIs
 * @ingroup example_apis
 * @{
 */

/**
 * @brief Custom subsystem interface routine
 *
 * @retval >= 0 If successful.
 * @retval Errno Negative errno code if failure.
 */
int32_t custom_subsys_said(void);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* EXAMPLE_APPLICATION_INCLUDE_EXAMPLE_APPLICATION_CUSTOM_SUBSYS_CUSTOM_SUBSYS_H_ */
