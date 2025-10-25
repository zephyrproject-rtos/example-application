/*
 * Copyright (c) 2025 James Walmsley
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>

int main(void)
{
	printk("Hello world from 2 %s\n", CONFIG_BOARD_TARGET);

	return 0;
}


