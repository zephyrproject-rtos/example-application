/*
 * Copyright (c) 2021 Legrand North America, LLC.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/** @file
 *  @brief Top level testing of Custom Subsys APIs
 */

#include <custom_subsys/custom_subsys.h>
#include <ztest.h>

static void test_custom_subsys_said(void)
{
	int32_t result = custom_subsys_said();
	zassert_true((result >= 0), "static status call failed");
}

void test_main(void)
{
	/*
	 * ztest_test_suite(...) macro has format restrictions, including:
	 *   - no conditional compiles within the ztest_test_suite(...) macro
	 *   - exactly one ztest_unit_test() per line.
	 *   - no comments on the line after ztest_unit_test().
	 *   ? test suite name must be on a different line than unit test(s)
	 * so auto-formatting needs to be disabled around these calls.
	 */

	ztest_test_suite(test_custom_subsys,

		ztest_unit_test(test_custom_subsys_said)
	);

	ztest_run_test_suite(test_custom_subsys);
}
