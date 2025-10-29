# Install script for directory: C:/zephyrproject/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Zephyr-Kernel")
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

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/Gabe/zephyr-sdk-0.17.4/x86_64-zephyr-elf/bin/x86_64-zephyr-elf-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/acpica/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/cmsis-dsp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/cmsis-nn/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/cmsis_6/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/adi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_afbr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_ambiq/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/atmel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_bouffalolab/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_espressif/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_ethos_u/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_gigadevice/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_infineon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_intel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/microchip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/nuvoton/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_nxp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/openisa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/quicklogic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_renesas/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_rpi_pico/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_sifli/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_silabs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_stm32/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_tdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_telink/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/ti/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_wch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hal_wurthelektronik/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/xtensa/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/hostap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/liblc3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/libmctp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/libsbc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/lora-basics-modem/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/nrf_wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/percepio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/picolibc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/trusted-firmware-a/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/trusted-firmware-m/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/uoscore-uedhoc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/zcbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/modules/nrf_hw_models/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/cmake/reports/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/Gabe/Desktop/Embarcados/zephyr-app-ufsm00292/APP_gfschuster/build/zephyr/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
