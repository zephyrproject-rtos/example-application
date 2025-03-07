#-------------------------------------------------------------------------------
# Zephyr Example Application
#
# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.3)

#-------------------------------------------
# Overlay File
set(DTC_OVERLAY_FILE "boards/esp32.overlay")
set(CONFIG_APPLICATION_DEFINED_SYSCALL  TRUE)

#-------------------------------------------
# Include Zephyr SDK
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

#-------------------------------------------
# Project
project(app LANGUAGES C)

#-------------------------------------------
# Source Files
target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE ../drivers/blink/gpio_led.c)

#-------------------------------------------
# Include Directories
zephyr_syscall_include_directories(./../include/app/drivers/)
zephyr_include_directories(./../include)
# target_include_directories(app PRIVATE ./../include/)

target_include_directories(app PRIVATE ./../include/app/drivers)

#-------------------------------------------
# System Calls Header Files
zephyr_syscall_header (./../include/app/drivers/blink.h)