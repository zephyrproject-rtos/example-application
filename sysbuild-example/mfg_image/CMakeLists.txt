# Copyright (c) 2025 James Walmsley <james@fullfat-fs.co.uk>
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZPEHYR_BASE})

project(mfg_image)
target_sources(app PRIVATE src/main.c)

