MEMORY
    {
    RAM (wx) : ORIGIN = ((0) + 0x100000), LENGTH = ((33554432) - 0x100000)
   
    IDT_LIST : ORIGIN = 0xFFFF1000, LENGTH = 2K
    }
  OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
  OUTPUT_ARCH("i386")
epoint = (("__start") - ((0x0 + 0x100000) - (0x0 + 0x100000)));
ENTRY(epoint)
SECTIONS
 {
 .rel.plt :
 {
 *(.rel.plt)
 PROVIDE_HIDDEN (__rel_iplt_start = .);
 *(.rel.iplt)
 PROVIDE_HIDDEN (__rel_iplt_end = .);
 }
 .rela.plt :
 {
 *(.rela.plt)
 PROVIDE_HIDDEN (__rela_iplt_start = .);
 *(.rela.iplt)
 PROVIDE_HIDDEN (__rela_iplt_end = .);
 }
 .rel.dyn :
 {
 *(.rel.*)
 }
 .rela.dyn :
 {
 *(.rela.*)
 }
    /DISCARD/ :
 {
 *(.plt)
 }
    /DISCARD/ :
 {
 *(.iplt)
 }

 . = ALIGN(8);
 text :
 {
 __text_region_start = .;
 z_mapped_start = .;
HIDDEN(__rom_start_address = .);
FILL(0x00);
. += 0x0 - (. - __rom_start_address);
. = ALIGN(4);
 KEEP(*(.text.__start))
 KEEP(*(.text.irqstubs))
 KEEP(*(".gnu.linkonce.t.exc_*_stub"))
 *(.text)
 *(".text.*")
 *(.gnu.linkonce.t.*)
 *(.init)
 *(.fini)
 *(.eini)
 . = ALIGN(0x1000);
 } > RAM
 __text_region_end = .;
 __text_region_size = __text_region_end - __text_region_start;
 __rodata_region_start = .;
 initlevel :
 {
  __init_start = .;
  __init_EARLY_start = .; KEEP(*(SORT(.z_init_EARLY_P_?_*))); KEEP(*(SORT(.z_init_EARLY_P_??_*))); KEEP(*(SORT(.z_init_EARLY_P_???_*)));
  __init_PRE_KERNEL_1_start = .; KEEP(*(SORT(.z_init_PRE_KERNEL_1_P_?_*))); KEEP(*(SORT(.z_init_PRE_KERNEL_1_P_??_*))); KEEP(*(SORT(.z_init_PRE_KERNEL_1_P_???_*)));
  __init_PRE_KERNEL_2_start = .; KEEP(*(SORT(.z_init_PRE_KERNEL_2_P_?_*))); KEEP(*(SORT(.z_init_PRE_KERNEL_2_P_??_*))); KEEP(*(SORT(.z_init_PRE_KERNEL_2_P_???_*)));
  __init_POST_KERNEL_start = .; KEEP(*(SORT(.z_init_POST_KERNEL_P_?_*))); KEEP(*(SORT(.z_init_POST_KERNEL_P_??_*))); KEEP(*(SORT(.z_init_POST_KERNEL_P_???_*)));
  __init_APPLICATION_start = .; KEEP(*(SORT(.z_init_APPLICATION_P_?_*))); KEEP(*(SORT(.z_init_APPLICATION_P_??_*))); KEEP(*(SORT(.z_init_APPLICATION_P_???_*)));
  __init_SMP_start = .; KEEP(*(SORT(.z_init_SMP_P_?_*))); KEEP(*(SORT(.z_init_SMP_P_??_*))); KEEP(*(SORT(.z_init_SMP_P_???_*)));
  __init_end = .;
 } > RAM
 device_area : { _device_list_start = .; KEEP(*(SORT(._device.static.*_?_*))); KEEP(*(SORT(._device.static.*_??_*))); KEEP(*(SORT(._device.static.*_???_*))); KEEP(*(SORT(._device.static.*_????_*))); KEEP(*(SORT(._device.static.*_?????_*))); _device_list_end = .;; } > RAM
 initlevel_error :
 {
  KEEP(*(SORT(.z_init_*)))
 }
 ASSERT(SIZEOF(initlevel_error) == 0, "Undefined initialization levels used.")
 app_shmem_regions :
 {
  __app_shmem_regions_start = .;
  KEEP(*(SORT(.app_regions.*)));
  __app_shmem_regions_end = .;
 } > RAM
 k_p4wq_initparam_area : { _k_p4wq_initparam_list_start = .; KEEP(*(SORT_BY_NAME(._k_p4wq_initparam.static.*))); _k_p4wq_initparam_list_end = .;; } > RAM
 _static_thread_data_area : { __static_thread_data_list_start = .; KEEP(*(SORT_BY_NAME(.__static_thread_data.static.*))); __static_thread_data_list_end = .;; } > RAM
 device_deps :
 {
__device_deps_start = .;
KEEP(*(SORT(.__device_deps_pass2*)));
__device_deps_end = .;
 } > RAM
shared_irq_driver_api_area : { _shared_irq_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._shared_irq_driver_api.static.*))); _shared_irq_driver_api_list_end = .;; } > RAM
crypto_driver_api_area : { _crypto_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._crypto_driver_api.static.*))); _crypto_driver_api_list_end = .;; } > RAM
adc_driver_api_area : { _adc_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._adc_driver_api.static.*))); _adc_driver_api_list_end = .;; } > RAM
auxdisplay_driver_api_area : { _auxdisplay_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._auxdisplay_driver_api.static.*))); _auxdisplay_driver_api_list_end = .;; } > RAM
bbram_driver_api_area : { _bbram_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._bbram_driver_api.static.*))); _bbram_driver_api_list_end = .;; } > RAM
bt_hci_driver_api_area : { _bt_hci_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._bt_hci_driver_api.static.*))); _bt_hci_driver_api_list_end = .;; } > RAM
can_driver_api_area : { _can_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._can_driver_api.static.*))); _can_driver_api_list_end = .;; } > RAM
cellular_driver_api_area : { _cellular_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._cellular_driver_api.static.*))); _cellular_driver_api_list_end = .;; } > RAM
charger_driver_api_area : { _charger_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._charger_driver_api.static.*))); _charger_driver_api_list_end = .;; } > RAM
clock_control_driver_api_area : { _clock_control_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._clock_control_driver_api.static.*))); _clock_control_driver_api_list_end = .;; } > RAM
comparator_driver_api_area : { _comparator_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._comparator_driver_api.static.*))); _comparator_driver_api_list_end = .;; } > RAM
coredump_driver_api_area : { _coredump_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._coredump_driver_api.static.*))); _coredump_driver_api_list_end = .;; } > RAM
counter_driver_api_area : { _counter_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._counter_driver_api.static.*))); _counter_driver_api_list_end = .;; } > RAM
crc_driver_api_area : { _crc_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._crc_driver_api.static.*))); _crc_driver_api_list_end = .;; } > RAM
dac_driver_api_area : { _dac_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._dac_driver_api.static.*))); _dac_driver_api_list_end = .;; } > RAM
dai_driver_api_area : { _dai_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._dai_driver_api.static.*))); _dai_driver_api_list_end = .;; } > RAM
display_driver_api_area : { _display_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._display_driver_api.static.*))); _display_driver_api_list_end = .;; } > RAM
dma_driver_api_area : { _dma_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._dma_driver_api.static.*))); _dma_driver_api_list_end = .;; } > RAM
edac_driver_api_area : { _edac_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._edac_driver_api.static.*))); _edac_driver_api_list_end = .;; } > RAM
eeprom_driver_api_area : { _eeprom_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._eeprom_driver_api.static.*))); _eeprom_driver_api_list_end = .;; } > RAM
emul_bbram_driver_api_area : { _emul_bbram_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._emul_bbram_driver_api.static.*))); _emul_bbram_driver_api_list_end = .;; } > RAM
fuel_gauge_emul_driver_api_area : { _fuel_gauge_emul_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._fuel_gauge_emul_driver_api.static.*))); _fuel_gauge_emul_driver_api_list_end = .;; } > RAM
emul_sensor_driver_api_area : { _emul_sensor_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._emul_sensor_driver_api.static.*))); _emul_sensor_driver_api_list_end = .;; } > RAM
entropy_driver_api_area : { _entropy_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._entropy_driver_api.static.*))); _entropy_driver_api_list_end = .;; } > RAM
espi_driver_api_area : { _espi_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._espi_driver_api.static.*))); _espi_driver_api_list_end = .;; } > RAM
espi_saf_driver_api_area : { _espi_saf_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._espi_saf_driver_api.static.*))); _espi_saf_driver_api_list_end = .;; } > RAM
flash_driver_api_area : { _flash_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._flash_driver_api.static.*))); _flash_driver_api_list_end = .;; } > RAM
fpga_driver_api_area : { _fpga_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._fpga_driver_api.static.*))); _fpga_driver_api_list_end = .;; } > RAM
fuel_gauge_driver_api_area : { _fuel_gauge_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._fuel_gauge_driver_api.static.*))); _fuel_gauge_driver_api_list_end = .;; } > RAM
gnss_driver_api_area : { _gnss_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._gnss_driver_api.static.*))); _gnss_driver_api_list_end = .;; } > RAM
gpio_driver_api_area : { _gpio_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._gpio_driver_api.static.*))); _gpio_driver_api_list_end = .;; } > RAM
haptics_driver_api_area : { _haptics_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._haptics_driver_api.static.*))); _haptics_driver_api_list_end = .;; } > RAM
hwspinlock_driver_api_area : { _hwspinlock_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._hwspinlock_driver_api.static.*))); _hwspinlock_driver_api_list_end = .;; } > RAM
i2c_driver_api_area : { _i2c_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._i2c_driver_api.static.*))); _i2c_driver_api_list_end = .;; } > RAM
i2c_target_driver_api_area : { _i2c_target_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._i2c_target_driver_api.static.*))); _i2c_target_driver_api_list_end = .;; } > RAM
i2s_driver_api_area : { _i2s_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._i2s_driver_api.static.*))); _i2s_driver_api_list_end = .;; } > RAM
i3c_driver_api_area : { _i3c_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._i3c_driver_api.static.*))); _i3c_driver_api_list_end = .;; } > RAM
ipm_driver_api_area : { _ipm_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._ipm_driver_api.static.*))); _ipm_driver_api_list_end = .;; } > RAM
led_driver_api_area : { _led_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._led_driver_api.static.*))); _led_driver_api_list_end = .;; } > RAM
led_strip_driver_api_area : { _led_strip_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._led_strip_driver_api.static.*))); _led_strip_driver_api_list_end = .;; } > RAM
lora_driver_api_area : { _lora_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._lora_driver_api.static.*))); _lora_driver_api_list_end = .;; } > RAM
mbox_driver_api_area : { _mbox_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._mbox_driver_api.static.*))); _mbox_driver_api_list_end = .;; } > RAM
mdio_driver_api_area : { _mdio_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._mdio_driver_api.static.*))); _mdio_driver_api_list_end = .;; } > RAM
mipi_dbi_driver_api_area : { _mipi_dbi_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._mipi_dbi_driver_api.static.*))); _mipi_dbi_driver_api_list_end = .;; } > RAM
mipi_dsi_driver_api_area : { _mipi_dsi_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._mipi_dsi_driver_api.static.*))); _mipi_dsi_driver_api_list_end = .;; } > RAM
mspi_driver_api_area : { _mspi_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._mspi_driver_api.static.*))); _mspi_driver_api_list_end = .;; } > RAM
opamp_driver_api_area : { _opamp_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._opamp_driver_api.static.*))); _opamp_driver_api_list_end = .;; } > RAM
peci_driver_api_area : { _peci_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._peci_driver_api.static.*))); _peci_driver_api_list_end = .;; } > RAM
ps2_driver_api_area : { _ps2_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._ps2_driver_api.static.*))); _ps2_driver_api_list_end = .;; } > RAM
ptp_clock_driver_api_area : { _ptp_clock_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._ptp_clock_driver_api.static.*))); _ptp_clock_driver_api_list_end = .;; } > RAM
pwm_driver_api_area : { _pwm_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._pwm_driver_api.static.*))); _pwm_driver_api_list_end = .;; } > RAM
regulator_parent_driver_api_area : { _regulator_parent_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._regulator_parent_driver_api.static.*))); _regulator_parent_driver_api_list_end = .;; } > RAM
regulator_driver_api_area : { _regulator_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._regulator_driver_api.static.*))); _regulator_driver_api_list_end = .;; } > RAM
reset_driver_api_area : { _reset_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._reset_driver_api.static.*))); _reset_driver_api_list_end = .;; } > RAM
retained_mem_driver_api_area : { _retained_mem_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._retained_mem_driver_api.static.*))); _retained_mem_driver_api_list_end = .;; } > RAM
rtc_driver_api_area : { _rtc_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._rtc_driver_api.static.*))); _rtc_driver_api_list_end = .;; } > RAM
sdhc_driver_api_area : { _sdhc_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._sdhc_driver_api.static.*))); _sdhc_driver_api_list_end = .;; } > RAM
sensor_driver_api_area : { _sensor_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._sensor_driver_api.static.*))); _sensor_driver_api_list_end = .;; } > RAM
smbus_driver_api_area : { _smbus_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._smbus_driver_api.static.*))); _smbus_driver_api_list_end = .;; } > RAM
spi_driver_api_area : { _spi_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._spi_driver_api.static.*))); _spi_driver_api_list_end = .;; } > RAM
stepper_driver_api_area : { _stepper_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._stepper_driver_api.static.*))); _stepper_driver_api_list_end = .;; } > RAM
syscon_driver_api_area : { _syscon_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._syscon_driver_api.static.*))); _syscon_driver_api_list_end = .;; } > RAM
tee_driver_api_area : { _tee_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._tee_driver_api.static.*))); _tee_driver_api_list_end = .;; } > RAM
video_driver_api_area : { _video_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._video_driver_api.static.*))); _video_driver_api_list_end = .;; } > RAM
virtio_driver_api_area : { _virtio_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._virtio_driver_api.static.*))); _virtio_driver_api_list_end = .;; } > RAM
w1_driver_api_area : { _w1_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._w1_driver_api.static.*))); _w1_driver_api_list_end = .;; } > RAM
wdt_driver_api_area : { _wdt_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._wdt_driver_api.static.*))); _wdt_driver_api_list_end = .;; } > RAM
can_transceiver_driver_api_area : { _can_transceiver_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._can_transceiver_driver_api.static.*))); _can_transceiver_driver_api_list_end = .;; } > RAM
nrf_clock_control_driver_api_area : { _nrf_clock_control_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._nrf_clock_control_driver_api.static.*))); _nrf_clock_control_driver_api_list_end = .;; } > RAM
i3c_target_driver_api_area : { _i3c_target_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._i3c_target_driver_api.static.*))); _i3c_target_driver_api_list_end = .;; } > RAM
its_driver_api_area : { _its_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._its_driver_api.static.*))); _its_driver_api_list_end = .;; } > RAM
vtd_driver_api_area : { _vtd_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._vtd_driver_api.static.*))); _vtd_driver_api_list_end = .;; } > RAM
renesas_elc_driver_api_area : { _renesas_elc_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._renesas_elc_driver_api.static.*))); _renesas_elc_driver_api_list_end = .;; } > RAM
tgpio_driver_api_area : { _tgpio_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._tgpio_driver_api.static.*))); _tgpio_driver_api_list_end = .;; } > RAM
pcie_ctrl_driver_api_area : { _pcie_ctrl_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._pcie_ctrl_driver_api.static.*))); _pcie_ctrl_driver_api_list_end = .;; } > RAM
pcie_ep_driver_api_area : { _pcie_ep_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._pcie_ep_driver_api.static.*))); _pcie_ep_driver_api_list_end = .;; } > RAM
psi5_driver_api_area : { _psi5_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._psi5_driver_api.static.*))); _psi5_driver_api_list_end = .;; } > RAM
sent_driver_api_area : { _sent_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._sent_driver_api.static.*))); _sent_driver_api_list_end = .;; } > RAM
svc_driver_api_area : { _svc_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._svc_driver_api.static.*))); _svc_driver_api_list_end = .;; } > RAM
uart_driver_api_area : { _uart_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._uart_driver_api.static.*))); _uart_driver_api_list_end = .;; } > RAM
bc12_emul_driver_api_area : { _bc12_emul_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._bc12_emul_driver_api.static.*))); _bc12_emul_driver_api_list_end = .;; } > RAM
bc12_driver_api_area : { _bc12_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._bc12_driver_api.static.*))); _bc12_driver_api_list_end = .;; } > RAM
usbc_ppc_driver_api_area : { _usbc_ppc_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._usbc_ppc_driver_api.static.*))); _usbc_ppc_driver_api_list_end = .;; } > RAM
tcpc_driver_api_area : { _tcpc_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._tcpc_driver_api.static.*))); _tcpc_driver_api_list_end = .;; } > RAM
usbc_vbus_driver_api_area : { _usbc_vbus_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._usbc_vbus_driver_api.static.*))); _usbc_vbus_driver_api_list_end = .;; } > RAM
ivshmem_driver_api_area : { _ivshmem_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._ivshmem_driver_api.static.*))); _ivshmem_driver_api_list_end = .;; } > RAM
ethphy_driver_api_area : { _ethphy_driver_api_list_start = .; KEEP(*(SORT_BY_NAME(._ethphy_driver_api.static.*))); _ethphy_driver_api_list_end = .;; } > RAM
ztest :
{
 _ztest_expected_result_entry_list_start = .; KEEP(*(SORT_BY_NAME(._ztest_expected_result_entry.static.*))); _ztest_expected_result_entry_list_end = .;;
 _ztest_suite_node_list_start = .; KEEP(*(SORT_BY_NAME(._ztest_suite_node.static.*))); _ztest_suite_node_list_end = .;;
 _ztest_unit_test_list_start = .; KEEP(*(SORT_BY_NAME(._ztest_unit_test.static.*))); _ztest_unit_test_list_end = .;;
 _ztest_test_rule_list_start = .; KEEP(*(SORT_BY_NAME(._ztest_test_rule.static.*))); _ztest_test_rule_list_end = .;;
} > RAM
 init_array :
 {
  __zephyr_init_array_start = .;
  KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)
   SORT_BY_INIT_PRIORITY(.ctors.*)))
  KEEP (*(.init_array .ctors))
  __zephyr_init_array_end = .;
 } > RAM
 ASSERT(__zephyr_init_array_start == __zephyr_init_array_end,
        "GNU-style constructors required but STATIC_INIT_GNU not enabled")
 bt_l2cap_fixed_chan_area : { _bt_l2cap_fixed_chan_list_start = .; KEEP(*(SORT_BY_NAME(._bt_l2cap_fixed_chan.static.*))); _bt_l2cap_fixed_chan_list_end = .;; } > RAM
 bt_gatt_service_static_area : { _bt_gatt_service_static_list_start = .; KEEP(*(SORT_BY_NAME(._bt_gatt_service_static.static.*))); _bt_gatt_service_static_list_end = .;; } > RAM
 log_strings_area : { _log_strings_list_start = .; KEEP(*(SORT_BY_NAME(._log_strings.static.*))); _log_strings_list_end = .;; } > RAM
 log_stmesp_ptr_area : { _log_stmesp_ptr_list_start = .; KEEP(*(SORT_BY_NAME(._log_stmesp_ptr.static.*))); _log_stmesp_ptr_list_end = .;; } > RAM
 log_stmesp_str_area : { _log_stmesp_str_list_start = .; KEEP(*(SORT_BY_NAME(._log_stmesp_str.static.*))); _log_stmesp_str_list_end = .;; } > RAM
 log_const_area : { _log_const_list_start = .; KEEP(*(SORT_BY_NAME(._log_const.static.*))); _log_const_list_end = .;; } > RAM
 log_backend_area : { _log_backend_list_start = .; KEEP(*(SORT_BY_NAME(._log_backend.static.*))); _log_backend_list_end = .;; } > RAM
 log_link_area : { _log_link_list_start = .; KEEP(*(SORT_BY_NAME(._log_link.static.*))); _log_link_list_end = .;; } > RAM
 tracing_backend_area : { _tracing_backend_list_start = .; KEEP(*(SORT_BY_NAME(._tracing_backend.static.*))); _tracing_backend_list_end = .;; } > RAM
 zephyr_dbg_info :
 {
  KEEP(*(".dbg_thread_info"));
 } > RAM
 intc_table_area : { _intc_table_list_start = .; KEEP(*(SORT_BY_NAME(._intc_table.static.*))); _intc_table_list_end = .;; } > RAM
 symbol_to_keep :
 {
  __symbol_to_keep_start = .;
  KEEP(*(SORT(.symbol_to_keep*)));
  __symbol_to_keep_end = .;
 } > RAM
 shell_area : { _shell_list_start = .; KEEP(*(SORT_BY_NAME(._shell.static.*))); _shell_list_end = .;; } > RAM
 shell_root_cmds_area : { _shell_root_cmds_list_start = .; KEEP(*(SORT_BY_NAME(._shell_root_cmds.static.*))); _shell_root_cmds_list_end = .;; } > RAM
 shell_subcmds_area : { _shell_subcmds_list_start = .; KEEP(*(SORT_BY_NAME(._shell_subcmds.static.*))); _shell_subcmds_list_end = .;; } > RAM
 shell_dynamic_subcmds_area : { _shell_dynamic_subcmds_list_start = .; KEEP(*(SORT_BY_NAME(._shell_dynamic_subcmds.static.*))); _shell_dynamic_subcmds_list_end = .;; } > RAM
 cfb_font_area : { _cfb_font_list_start = .; KEEP(*(SORT_BY_NAME(._cfb_font.static.*))); _cfb_font_list_end = .;; } > RAM
 rodata :
 {
 *(.rodata)
 *(".rodata.*")
 *(.gnu.linkonce.r.*)
 . = ALIGN(8);
 _idt_base_address = .;
 KEEP(*(staticIdt))
 . = ALIGN(4);
 _irq_to_interrupt_vector = .;
 KEEP(*(irq_int_vector_map))
 } > RAM
 /DISCARD/ : { *(.eh_frame) }
 . = ALIGN(0x1000);
 __rodata_region_end = .;
 __rodata_region_size = __rodata_region_end - __rodata_region_start;

 /DISCARD/ : {
  *(.got.plt)
  *(.igot.plt)
  *(.got)
  *(.igot)
 }

 datas :
 {
 . = ALIGN(0x1000);
 __data_region_start = .;
 __data_start = .;
 *(.data)
 *(".data.*")
 *(".kernel.*")
 . = ALIGN(4);
 __data_end = .;
 } > RAM
 __data_size = __data_end - __data_start;
 __data_load_start = LOADADDR(datas);
 __data_region_load_start = LOADADDR(datas);
        device_states :
        {
  . = ALIGN(4);
                __device_states_start = .;
  KEEP(*(".z_devstate"));
  KEEP(*(".z_devstate.*"));
                __device_states_end = .;
  . = ALIGN(4);
        } > RAM
 log_mpsc_pbuf_area : { _log_mpsc_pbuf_list_start = .; *(SORT_BY_NAME(._log_mpsc_pbuf.static.*)); _log_mpsc_pbuf_list_end = .;; } > RAM
 log_msg_ptr_area : { _log_msg_ptr_list_start = .; KEEP(*(SORT_BY_NAME(._log_msg_ptr.static.*))); _log_msg_ptr_list_end = .;; } > RAM
 log_dynamic_area : { _log_dynamic_list_start = .; KEEP(*(SORT_BY_NAME(._log_dynamic.static.*))); _log_dynamic_list_end = .;; } > RAM
 k_timer_area : { _k_timer_list_start = .; *(SORT_BY_NAME(._k_timer.static.*)); _k_timer_list_end = .;; } > RAM
 k_mem_slab_area : { _k_mem_slab_list_start = .; *(SORT_BY_NAME(._k_mem_slab.static.*)); _k_mem_slab_list_end = .;; } > RAM
 k_heap_area : { _k_heap_list_start = .; *(SORT_BY_NAME(._k_heap.static.*)); _k_heap_list_end = .;; } > RAM
 k_mutex_area : { _k_mutex_list_start = .; *(SORT_BY_NAME(._k_mutex.static.*)); _k_mutex_list_end = .;; } > RAM
 k_stack_area : { _k_stack_list_start = .; *(SORT_BY_NAME(._k_stack.static.*)); _k_stack_list_end = .;; } > RAM
 k_msgq_area : { _k_msgq_list_start = .; *(SORT_BY_NAME(._k_msgq.static.*)); _k_msgq_list_end = .;; } > RAM
 k_mbox_area : { _k_mbox_list_start = .; *(SORT_BY_NAME(._k_mbox.static.*)); _k_mbox_list_end = .;; } > RAM
 k_pipe_area : { _k_pipe_list_start = .; *(SORT_BY_NAME(._k_pipe.static.*)); _k_pipe_list_end = .;; } > RAM
 k_sem_area : { _k_sem_list_start = .; *(SORT_BY_NAME(._k_sem.static.*)); _k_sem_list_end = .;; } > RAM
 k_event_area : { _k_event_list_start = .; *(SORT_BY_NAME(._k_event.static.*)); _k_event_list_end = .;; } > RAM
 k_queue_area : { _k_queue_list_start = .; *(SORT_BY_NAME(._k_queue.static.*)); _k_queue_list_end = .;; } > RAM
 k_fifo_area : { _k_fifo_list_start = .; *(SORT_BY_NAME(._k_fifo.static.*)); _k_fifo_list_end = .;; } > RAM
 k_lifo_area : { _k_lifo_list_start = .; *(SORT_BY_NAME(._k_lifo.static.*)); _k_lifo_list_end = .;; } > RAM
 k_condvar_area : { _k_condvar_list_start = .; *(SORT_BY_NAME(._k_condvar.static.*)); _k_condvar_list_end = .;; } > RAM
 sys_mem_blocks_ptr_area : { _sys_mem_blocks_ptr_list_start = .; *(SORT_BY_NAME(._sys_mem_blocks_ptr.static.*)); _sys_mem_blocks_ptr_list_end = .;; } > RAM
 net_buf_pool_area : { _net_buf_pool_list_start = .; KEEP(*(SORT_BY_NAME(._net_buf_pool.static.*))); _net_buf_pool_list_end = .;; } > RAM
    pagetables :
    {
    . = ALIGN(4096);
    z_x86_pagetables_start = .;
    KEEP(*(pagetables))
    z_x86_kernel_ptables = . - 32;
    } > RAM
    /DISCARD/ :
    {
    *(.dummy_pagetables)
    }
 . = ALIGN(0x1000);
 __data_region_end = .;
 bss (NOLOAD) :
 {
 . = ALIGN(0x1000);
 _image_ram_start = .;
 . = ALIGN(4);
 __kernel_ram_start = .;
 __bss_start = .;
 *(.bss)
 *(".bss.*")
 *(COMMON)
 *(".kernel_bss.*")
 . = ALIGN(4);
 __bss_end = .;
 } > RAM
 __bss_num_words = (__bss_end - __bss_start) >> 2;
noinit (NOLOAD) :
{
        *(.noinit)
        *(".noinit.*")
} > RAM
 . = ALIGN(0x1000);
 __kernel_ram_end = ((0) + 0x100000) + ((33554432) - 0x100000);
 __kernel_ram_size = __kernel_ram_end - __kernel_ram_start;
 _image_ram_all = (((0) + 0x100000) + ((33554432) - 0x100000)) - _image_ram_start;
 z_mapped_end = .;
 z_mapped_size = z_mapped_end - z_mapped_start;
 /DISCARD/ :
 {
 KEEP(*(.spurIsr))
 KEEP(*(.spurNoErrIsr))
 KEEP(*(.intList))
 KEEP(*(.gnu.linkonce.intList.*))
 }
    .last_ram_section (NOLOAD) :
    {
 _image_ram_end = .;
 _image_ram_size = _image_ram_end - _image_ram_start;
 _end = .;
 z_mapped_end = .;
    } > RAM
   
 .stab 0 : { *(.stab) }
 .stabstr 0 : { *(.stabstr) }
 .stab.excl 0 : { *(.stab.excl) }
 .stab.exclstr 0 : { *(.stab.exclstr) }
 .stab.index 0 : { *(.stab.index) }
 .stab.indexstr 0 : { *(.stab.indexstr) }
 .gnu.build.attributes 0 : { *(.gnu.build.attributes .gnu.build.attributes.*) }
 .comment 0 : { *(.comment) }
 .debug 0 : { *(.debug) }
 .line 0 : { *(.line) }
 .debug_srcinfo 0 : { *(.debug_srcinfo) }
 .debug_sfnames 0 : { *(.debug_sfnames) }
 .debug_aranges 0 : { *(.debug_aranges) }
 .debug_pubnames 0 : { *(.debug_pubnames) }
 .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
 .debug_abbrev 0 : { *(.debug_abbrev) }
 .debug_line 0 : { *(.debug_line .debug_line.* .debug_line_end ) }
 .debug_frame 0 : { *(.debug_frame) }
 .debug_str 0 : { *(.debug_str) }
 .debug_loc 0 : { *(.debug_loc) }
 .debug_macinfo 0 : { *(.debug_macinfo) }
 .debug_weaknames 0 : { *(.debug_weaknames) }
 .debug_funcnames 0 : { *(.debug_funcnames) }
 .debug_typenames 0 : { *(.debug_typenames) }
 .debug_varnames 0 : { *(.debug_varnames) }
 .debug_pubtypes 0 : { *(.debug_pubtypes) }
 .debug_ranges 0 : { *(.debug_ranges) }
 .debug_addr 0 : { *(.debug_addr) }
 .debug_line_str 0 : { *(.debug_line_str) }
 .debug_loclists 0 : { *(.debug_loclists) }
 .debug_macro 0 : { *(.debug_macro) }
 .debug_names 0 : { *(.debug_names) }
 .debug_rnglists 0 : { *(.debug_rnglists) }
 .debug_str_offsets 0 : { *(.debug_str_offsets) }
 .debug_sup 0 : { *(.debug_sup) }
 /DISCARD/ : { *(.note.GNU-stack) }
 .symtab 0 : { *(.symtab) }
 .strtab 0 : { *(.strtab) }
 .shstrtab 0 : { *(.shstrtab) }

}
