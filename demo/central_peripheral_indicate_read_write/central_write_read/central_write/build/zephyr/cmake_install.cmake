# Install script for directory: /home/jordan/ncs/v2.5.1/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/jordan/ncs/toolchains/7795df4459/opt/zephyr-sdk/arm-zephyr-eabi/bin/arm-zephyr-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/nrf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/wfa-qt-control-app/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/trusted-firmware-m/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/cjson/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/azure-sdk-for-c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/cirrus-logic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/memfault-firmware-sdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/canopennode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/chre/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/hal_wurthelektronik/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/liblc3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/lz4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/nrf_hw_models/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/picolibc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/tinycrypt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/TraceRecorder/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/uoscore-uedhoc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/zcbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/zscilib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/nrfxlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/modules/connectedhomeip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/jordan/Documents/PFE/appli/APPLICATION_WORKING/central_peripheral_indicate_read_write/central_write_read/central_write/build/zephyr/cmake/reports/cmake_install.cmake")
endif()

