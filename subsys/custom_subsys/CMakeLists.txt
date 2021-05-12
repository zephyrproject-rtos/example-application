# SPDX-License-Identifier: Apache-2.0

zephyr_interface_library_named(custom_subsys)

zephyr_library()
zephyr_library_sources(custom_subsys.c)

zephyr_library_link_libraries(custom_subsys)

add_definitions(-DMODULE_NAME="${module_name}")

