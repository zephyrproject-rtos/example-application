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

# Define additional overlay configuration files
# set(OVERLAY_CONFIG "overlay-debug.conf; overlay-enterprise-variable-bufs.conf; overlay-enterprise.conf;")

# Pass configuration files to the build system
# set_property(GLOBAL PROPERTY OVERLAY_CONFIG "${OVERLAY_CONFIG}")

#-------------------------------------------
# Include Zephyr SDK
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

#-------------------------------------------
# Project
project(app)

#-------------------------------------------
# Source Files
target_sources(app PRIVATE src/main.cpp)
target_sources(app PRIVATE src/handlers.cpp)
target_sources(app PRIVATE src/wifi.cpp)
# Manager Factory
target_sources(app PRIVATE src/managerFactory/managerFactory.cpp)
# Wifi Manager
target_sources(app PRIVATE src/wifiManager/wifiManager.cpp)
# Wifi State Machine
target_sources(app PRIVATE src/wifiSM/wifiContext.cpp)
target_sources(app PRIVATE src/wifiSM/wifiStateConnected.cpp)
target_sources(app PRIVATE src/wifiSM/wifiStateConnecting.cpp)
target_sources(app PRIVATE src/wifiSM/wifiStateDisconnected.cpp)
target_sources(app PRIVATE src/wifiSM/wifiStateIdle.cpp)

# target_sources(app PRIVATE src/my_syscall.c)
# target_sources(app PRIVATE ../drivers/blink/gpio_led.c)

#-------------------------------------------
# Include Directories
# zephyr_syscall_include_directories(./../include/app/drivers/)
# zephyr_include_directories(./../include)
# zephyr_include_directories(./../include/app/drivers/)

target_include_directories(app PRIVATE src/managerFactory)
target_include_directories(app PRIVATE src/wifiManager)
target_include_directories(app PRIVATE src/wifiSM)

target_include_directories(app PRIVATE ./../include/)
target_include_directories(app PRIVATE ./../include/app/)
target_include_directories(app PRIVATE ./../include/app/drivers)
target_include_directories(app PRIVATE ./../include/utils/myLogger)

#-------------------------------------------
# System Calls Header Files
# zephyr_syscall_header_ifdef (./../include/app/drivers/my_syscall.h)
# zephyr_syscall_header (./../include/app/drivers/blink.h)

#-------------------------------------------
# Libraries
target_link_libraries(app PRIVATE zephyr_interface)

#-------------------------------------------
# Compiler Options
target_compile_definitions(app PRIVATE
    WIFI_SSID="$ENV{WIFI_SSID}"
    WIFI_PASSWORD="$ENV{WIFI_PASSWORD}"
)