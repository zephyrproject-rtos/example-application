# SPDX-License-Identifier: Apache-2.0
# Author: James Walmsley <james@fullfat-fs.co.uk>

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
test_sysbuild(REQUIRED)

project(sysbuild-example)

target_sources(app PRIVATE main_image/src/main.c)

