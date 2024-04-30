# Copyright (c) 2024 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_iterable_section(
  NAME blink_driver_api
  KVMA RAM_REGION GROUP RODATA_REGION
  SUBALIGN CONFIG_LINKER_ITERABLE_SUBALIGN)

zephyr_library_sources_ifdef(CONFIG_BLINK_GPIO_LED gpio_led.c)
