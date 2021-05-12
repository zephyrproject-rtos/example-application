/*
 * Copyright (c) 2021 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/** @file
 *  @brief Top level testing of Custom Subsys APIs
 */

#include <example-application/custom_subsys/custom_subsys.h>
#include <zephyr/ztest.h>

ZTEST(suite1, test_custom_subsys_said)
{
	int32_t result = custom_subsys_said();
	zassert_true((result >= 0), "static status call failed");
}

ZTEST_SUITE(suite1, NULL, NULL, NULL, NULL, NULL);
