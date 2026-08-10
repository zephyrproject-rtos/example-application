/*
 * Copyright (c) 2025 James Walmsley
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>

int main(void)
{
	printk("Manufacturing image on: %s\n", CONFIG_BOARD_TARGET);

	return 0;
}

