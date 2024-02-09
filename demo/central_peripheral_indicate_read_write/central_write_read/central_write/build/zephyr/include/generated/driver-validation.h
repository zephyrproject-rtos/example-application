#ifndef DRIVER_VALIDATION_GEN_H
#define DRIVER_VALIDATION_GEN_H
#define Z_SYSCALL_DRIVER_GEN(ptr, op, driver_lower_case, driver_upper_case) \
		(Z_SYSCALL_OBJ(ptr, K_OBJ_DRIVER_##driver_upper_case) || \
		 Z_SYSCALL_DRIVER_OP(ptr, driver_lower_case##_driver_api, op))
                
#define Z_SYSCALL_DRIVER_ENTROPY(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, entropy, ENTROPY)

#define Z_SYSCALL_DRIVER_GPIO(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, gpio, GPIO)

#define Z_SYSCALL_DRIVER_MBOX(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, mbox, MBOX)

#define Z_SYSCALL_DRIVER_UART(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, uart, UART)

#define Z_SYSCALL_DRIVER_CRYPTO(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, crypto, CRYPTO)

#define Z_SYSCALL_DRIVER_ADC(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, adc, ADC)

#define Z_SYSCALL_DRIVER_AUXDISPLAY(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, auxdisplay, AUXDISPLAY)

#define Z_SYSCALL_DRIVER_BBRAM(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, bbram, BBRAM)

#define Z_SYSCALL_DRIVER_CAN(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, can, CAN)

#define Z_SYSCALL_DRIVER_COREDUMP(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, coredump, COREDUMP)

#define Z_SYSCALL_DRIVER_COUNTER(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, counter, COUNTER)

#define Z_SYSCALL_DRIVER_DAC(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, dac, DAC)

#define Z_SYSCALL_DRIVER_DAI(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, dai, DAI)

#define Z_SYSCALL_DRIVER_DMA(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, dma, DMA)

#define Z_SYSCALL_DRIVER_EDAC(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, edac, EDAC)

#define Z_SYSCALL_DRIVER_EEPROM(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, eeprom, EEPROM)

#define Z_SYSCALL_DRIVER_FUEL_GAUGE_EMUL(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, fuel_gauge_emul, FUEL_GAUGE_EMUL)

#define Z_SYSCALL_DRIVER_EMUL_SENSOR_BACKEND_API(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, emul_sensor_backend_api, EMUL_SENSOR_BACKEND_API)

#define Z_SYSCALL_DRIVER_ESPI(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, espi, ESPI)

#define Z_SYSCALL_DRIVER_ESPI_SAF(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, espi_saf, ESPI_SAF)

#define Z_SYSCALL_DRIVER_FLASH(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, flash, FLASH)

#define Z_SYSCALL_DRIVER_FPGA(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, fpga, FPGA)

#define Z_SYSCALL_DRIVER_FUEL_GAUGE(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, fuel_gauge, FUEL_GAUGE)

#define Z_SYSCALL_DRIVER_HWSPINLOCK(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, hwspinlock, HWSPINLOCK)

#define Z_SYSCALL_DRIVER_I2C(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, i2c, I2C)

#define Z_SYSCALL_DRIVER_I2S(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, i2s, I2S)

#define Z_SYSCALL_DRIVER_I3C(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, i3c, I3C)

#define Z_SYSCALL_DRIVER_IPM(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, ipm, IPM)

#define Z_SYSCALL_DRIVER_KSCAN(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, kscan, KSCAN)

#define Z_SYSCALL_DRIVER_LED(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, led, LED)

#define Z_SYSCALL_DRIVER_MDIO(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, mdio, MDIO)

#define Z_SYSCALL_DRIVER_MIPI_DSI(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, mipi_dsi, MIPI_DSI)

#define Z_SYSCALL_DRIVER_PECI(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, peci, PECI)

#define Z_SYSCALL_DRIVER_PS2(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, ps2, PS2)

#define Z_SYSCALL_DRIVER_PTP_CLOCK(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, ptp_clock, PTP_CLOCK)

#define Z_SYSCALL_DRIVER_PWM(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, pwm, PWM)

#define Z_SYSCALL_DRIVER_REGULATOR_PARENT(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, regulator_parent, REGULATOR_PARENT)

#define Z_SYSCALL_DRIVER_REGULATOR(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, regulator, REGULATOR)

#define Z_SYSCALL_DRIVER_RESET(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, reset, RESET)

#define Z_SYSCALL_DRIVER_RETAINED_MEM(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, retained_mem, RETAINED_MEM)

#define Z_SYSCALL_DRIVER_RTC(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, rtc, RTC)

#define Z_SYSCALL_DRIVER_SDHC(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, sdhc, SDHC)

#define Z_SYSCALL_DRIVER_SENSOR(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, sensor, SENSOR)

#define Z_SYSCALL_DRIVER_SMBUS(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, smbus, SMBUS)

#define Z_SYSCALL_DRIVER_SPI(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, spi, SPI)

#define Z_SYSCALL_DRIVER_SYSCON(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, syscon, SYSCON)

#define Z_SYSCALL_DRIVER_W1(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, w1, W1)

#define Z_SYSCALL_DRIVER_WDT(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, wdt, WDT)

#define Z_SYSCALL_DRIVER_CAN_TRANSCEIVER(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, can_transceiver, CAN_TRANSCEIVER)

#define Z_SYSCALL_DRIVER_UART_MUX(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, uart_mux, UART_MUX)

#define Z_SYSCALL_DRIVER_ITS(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, its, ITS)

#define Z_SYSCALL_DRIVER_PCIE_CTRL(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, pcie_ctrl, PCIE_CTRL)

#define Z_SYSCALL_DRIVER_SVC(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, svc, SVC)

#define Z_SYSCALL_DRIVER_BC12_EMUL(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, bc12_emul, BC12_EMUL)

#define Z_SYSCALL_DRIVER_BC12(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, bc12, BC12)

#define Z_SYSCALL_DRIVER_TCPC(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, tcpc, TCPC)

#define Z_SYSCALL_DRIVER_IVSHMEM(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, ivshmem, IVSHMEM)

#define Z_SYSCALL_DRIVER_EC_HOST_CMD_BACKEND_API(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, ec_host_cmd_backend_api, EC_HOST_CMD_BACKEND_API)

#define Z_SYSCALL_DRIVER_ETHPHY(ptr, op) Z_SYSCALL_DRIVER_GEN(ptr, op, ethphy, ETHPHY)
#endif /* DRIVER_VALIDATION_GEN_H */
