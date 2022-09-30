# Copyright (c) 2021 Nordic Semiconductor ASA
# Copyright (c) 2022 Legrand North America, LLC.
# SPDX-License-Identifier: Apache-2.0
#
# This CMake file is picked by the Zephyr build system because it is defined
# as the module CMake entry point (see zephyr/module.yml).


# Verify the module name is set correctly
set(expected_module_name "example-application")
if(NOT "${module_name}" STREQUAL ${expected_module_name})
  message(FATAL_ERROR "
    ------------------------------------------------------------------------
    A module configuration error has occurred.
    
    This module is named '${module_name}' when '${expected_module_name}' is expected. 
    Verify `module.yml` contains the line 'name: ${expected_module_name}'.
    ------------------------------------------------------------------------")
endif()
unset(expected_module_name)


zephyr_include_directories(include)

add_subdirectory(drivers)
add_subdirectory(lib)
