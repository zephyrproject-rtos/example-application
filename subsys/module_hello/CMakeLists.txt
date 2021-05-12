# SPDX-License-Identifier: Apache-2.0

zephyr_interface_library_named(module_hello)

zephyr_library()
zephyr_library_sources(hello.c)

zephyr_library_link_libraries(module_hello)

add_definitions(-DMODULE_NAME="${module_name}")

