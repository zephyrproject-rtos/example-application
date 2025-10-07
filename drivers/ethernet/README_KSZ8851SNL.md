# KSZ8851SNL Ethernet Driver for Zephyr RTOS

This driver provides support for the Microchip KSZ8851SNL Stand-Alone 10/100BASE-T Ethernet Controller with SPI interface for Zephyr RTOS, specifically targeting the SAMD21 microcontroller platform.

## Features

- SPI-based communication with KSZ8851SNL controller
- Full duplex and auto-negotiation support
- Interrupt-driven RX/TX operation
- Hardware and software reset support
- DHCP client support
- MAC address configuration (random or user-specified)
- Network statistics tracking

## Hardware Requirements

- Microchip KSZ8851SNL Ethernet Controller
- SAMD21 microcontroller (or compatible Zephyr-supported board)
- SPI bus connection
- GPIO pins for:
  - Interrupt line (INT pin - active low)
  - Reset line (RST pin - active low, optional)
  - SPI Chip Select (CS)

## Typical Hardware Connections

| KSZ8851SNL Pin | SAMD21 Pin | Function |
|----------------|------------|----------|
| SDI (MOSI)     | SERCOM PAD | SPI MOSI |
| SDO (MISO)     | SERCOM PAD | SPI MISO |
| SCLK           | SERCOM PAD | SPI SCK  |
| CS#            | GPIO       | Chip Select (active low) |
| INT#           | GPIO       | Interrupt (active low) |
| RST#           | GPIO       | Reset (active low, optional) |

## Configuration

### 1. Device Tree Configuration

Create a board overlay file (e.g., `app/boards/samd21_xplained_pro.overlay`):

```dts
&pinctrl {
    sercom4_spi_default: sercom4_spi_default {
        group1 {
            pinmux = <PA12C_SERCOM4_PAD0>,  /* MISO */
                     <PB10D_SERCOM4_PAD2>,  /* MOSI */
                     <PB11D_SERCOM4_PAD3>;  /* SCK */
        };
    };
};

&sercom4 {
    status = "okay";
    compatible = "atmel,sam0-spi";
    dipo = <0>;  /* MISO on PAD0 */
    dopo = <1>;  /* MOSI on PAD2, SCK on PAD3 */
    #address-cells = <1>;
    #size-cells = <0>;

    pinctrl-0 = <&sercom4_spi_default>;
    pinctrl-names = "default";

    cs-gpios = <&porta 18 GPIO_ACTIVE_LOW>;

    ksz8851snl: ksz8851snl@0 {
        compatible = "microchip,ksz8851snl";
        reg = <0>;
        spi-max-frequency = <25000000>;  /* 25 MHz max */
        int-gpios = <&porta 19 GPIO_ACTIVE_LOW>;
        reset-gpios = <&porta 20 GPIO_ACTIVE_LOW>;

        /* Optional: specify MAC address */
        /* local-mac-address = [00 04 A3 12 34 56]; */
    };
};

&porta {
    status = "okay";
};

&portb {
    status = "okay";
};
```

**Important**: Adjust the following for your hardware:
- SERCOM number (sercom4 in the example)
- Pin assignments (PA12, PB10, PB11, PA18, PA19, PA20)
- PAD configuration (dipo/dopo values)

### 2. Kconfig Configuration

Add to your `prj.conf`:

```conf
# Networking
CONFIG_NETWORKING=y
CONFIG_NET_L2_ETHERNET=y

# Ethernet driver
CONFIG_ETH_DRIVER=y
CONFIG_ETH_KSZ8851SNL=y

# SPI
CONFIG_SPI=y

# Network protocols
CONFIG_NET_IPV4=y
CONFIG_NET_DHCPV4=y

# Optional: Logging
CONFIG_LOG=y
CONFIG_ETHERNET_LOG_LEVEL_DBG=y
CONFIG_NET_LOG=y

# Optional: Network shell for debugging
CONFIG_NET_SHELL=y
CONFIG_SHELL=y
```

### 3. Driver-Specific Options

Available Kconfig options:

- `CONFIG_ETH_KSZ8851SNL_RX_THREAD_STACK_SIZE`: Stack size for RX thread (default: 800)
- `CONFIG_ETH_KSZ8851SNL_RX_THREAD_PRIO`: Priority for RX thread (default: 2)
- `CONFIG_ETH_KSZ8851SNL_TIMEOUT`: IP buffer timeout in milliseconds (default: 100)

### 4. Device Tree Properties

Supported properties in device tree:

| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `compatible` | string | Yes | Must be `"microchip,ksz8851snl"` |
| `reg` | int | Yes | SPI chip select index |
| `spi-max-frequency` | int | Yes | Maximum SPI frequency (25 MHz max) |
| `int-gpios` | phandle-array | Yes | Interrupt GPIO (active low) |
| `reset-gpios` | phandle-array | No | Reset GPIO (active low) |
| `full-duplex` | boolean | No | Force full duplex mode |
| `local-mac-address` | byte-array | No | MAC address (if not set, random MAC is generated) |

## Building

To build for SAMD21 Xplained Pro board:

```bash
west build -b samd21_xplained_pro app
```

For custom boards, ensure your board overlay is placed in:
- `app/boards/<board_name>.overlay`, or
- Specified via `-DDTC_OVERLAY_FILE`

## Usage Example

```c
#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet.h>

int main(void)
{
    struct net_if *iface;

    /* Get default network interface */
    iface = net_if_get_default();
    if (!iface) {
        printk("No network interface found\n");
        return -1;
    }

    /* Start DHCP */
    net_dhcpv4_start(iface);

    /* Your application code here */

    return 0;
}
```

## Network Shell Commands

With `CONFIG_NET_SHELL=y`, you can use these commands:

```bash
# Show network interfaces
net iface

# Show interface details
net iface show

# Show IPv4 addresses
net ipv4

# Ping test
net ping <ip_address>

# Show Ethernet statistics
net stats
```

## Troubleshooting

### Driver doesn't initialize

1. **Check SPI bus**: Verify SPI is properly configured and pins are correct
   ```bash
   # In shell
   device list
   ```

2. **Check chip ID**: Driver verifies chip ID (0x8870) on startup
   - Ensure proper SPI connections
   - Check SPI clock polarity and phase
   - Verify voltage levels (3.3V)

3. **Check interrupt pin**: Ensure INT pin is configured as active low

### No network connectivity

1. **Check link status**: Use `net iface` command to verify link is up

2. **Check DHCP**: Verify DHCP server is available on your network

3. **Enable debug logging**:
   ```conf
   CONFIG_ETHERNET_LOG_LEVEL_DBG=y
   CONFIG_NET_LOG=y
   CONFIG_LOG_MODE_IMMEDIATE=y
   ```

### SPI communication issues

- KSZ8851SNL supports up to 25 MHz SPI clock
- SPI Mode 0 (CPOL=0, CPHA=0) or Mode 3 (CPOL=1, CPHA=1) are supported
- Ensure proper signal integrity for long SPI traces

## Register Map

Key registers used by the driver:

| Register | Address | Description |
|----------|---------|-------------|
| REG_CHIP_ID | 0x00C0 | Chip ID (0x8870) |
| REG_MAC_ADDR_0/2/4 | 0x0010-0x0014 | MAC address |
| REG_TX_CTRL | 0x0070 | Transmit control |
| REG_RX_CTRL1 | 0x0074 | Receive control |
| REG_INT_STATUS | 0x009C | Interrupt status |
| REG_INT_ENABLE | 0x009A | Interrupt enable |

## References

- [KSZ8851SNL Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/00002381C.pdf)
- [Zephyr Networking Documentation](https://docs.zephyrproject.org/latest/connectivity/networking/index.html)
- [Zephyr Ethernet Driver API](https://docs.zephyrproject.org/latest/hardware/peripherals/ethernet.html)

## License

Apache-2.0

## Contributing

This driver is part of the Zephyr RTOS project.
