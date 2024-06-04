/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_DRIVERS_BLINK_H_
#define APP_DRIVERS_BLINK_H_

#include <zephyr/device.h>
#include <zephyr/toolchain.h>

/**
 * @defgroup drivers_blink Blink drivers
 * @ingroup drivers
 * @{
 *
 * @brief A custom driver class to blink LEDs
 *
 * This driver class is provided as an example of how to create custom driver
 * classes. It provides an interface to blink an LED at a configurable rate.
 * Implementations could include simple GPIO-controlled LEDs, addressable LEDs,
 * etc.
 */

/**
 * @defgroup drivers_blink_ops Blink driver operations
 * @{
 *
 * @brief Operations of the blink driver class.
 *
 * Each driver class tipically provides a set of operations that need to be
 * implemented by each driver. These are used to implement the public API. If
 * support for system calls is needed, the operations structure must be tagged
 * with `__subsystem` and follow the `${class}_driver_api` naming scheme.
 */

/** @brief Blink driver class operations */
__subsystem struct blink_driver_api {
	/**
	 * @brief Configure the LED blink period.
	 *
	 * @param dev Blink device instance.
	 * @param period_ms Period of the LED blink in milliseconds, 0 to
	 * disable blinking.
	 *
	 * @retval 0 if successful.
	 * @retval -EINVAL if @p period_ms can not be set.
	 * @retval -errno Other negative errno code on failure.
	 */
	int (*set_period_ms)(const struct device *dev, unsigned int period_ms);
};

/** @} */

/**
 * @defgroup drivers_blink_api Blink driver API
 * @{
 *
 * @brief Public API provided by the blink driver class.
 *
 * The public API is the interface that is used by applications to interact with
 * devices that implement the blink driver class. If support for system calls is
 * needed, functions accessing device fields need to be tagged with `__syscall`
 * and provide an implementation that follows the `z_impl_${function_name}`
 * naming scheme.
 */

/**
 * @brief Configure the LED blink period.
 *
 *
 * @param dev Blink device instance.
 * @param period_ms Period of the LED blink in milliseconds.
 *
 * @retval 0 if successful.
 * @retval -EINVAL if @p period_ms can not be set.
 * @retval -errno Other negative errno code on failure.
 */
__syscall int blink_set_period_ms(const struct device *dev,
				  unsigned int period_ms);

static inline int z_impl_blink_set_period_ms(const struct device *dev,
					     unsigned int period_ms)
{
	const struct blink_driver_api *api =
		(const struct blink_driver_api *)dev->api;

	return api->set_period_ms(dev, period_ms);
}

/**
 * @brief Turn LED blinking off.
 *
 * This is a convenience function to turn off the LED blinking. It also shows
 * how to create convenience functions that re-use other driver functions, or
 * driver operations, to provide a higher-level API.
 *
 * @param dev Blink device instance.
 *
 * @return See blink_set_period_ms().
 */
static inline int blink_off(const struct device *dev)
{
	return blink_set_period_ms(dev, 0);
}

#include <syscalls/blink.h>

/** @} */

/** @} */

#endif /* APP_DRIVERS_BLINK_H_ */
