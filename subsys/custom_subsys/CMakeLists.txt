# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources(custom_subsys.c)
zephyr_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/include/custom_subsys)

add_compile_definitions(MODULE_NAME="${module_name}")
