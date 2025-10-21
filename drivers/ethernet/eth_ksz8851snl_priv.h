/* KSZ8851SNL Stand-alone Ethernet Controller with SPI
 *
 * Copyright (c) 2024 Zephyr Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#ifndef _KSZ8851SNL_
#define _KSZ8851SNL_

/* Byte Enable and SPI Opcodes */
#define CMD_READ         0x0000
#define CMD_WRITE        0x8000
#define ADDR_SHIFT       2
#define ADDR_MASK        0x03FF
#define BE3              0x1000  /* Byte Enable 3 (MSB) */
#define BE2              0x0800  /* Byte Enable 2 */
#define BE1              0x0400  /* Byte Enable 1 */
#define BE0              0x0200  /* Byte Enable 0 (LSB) */

/* Register Addresses */
#define REG_CHIP_ID      0x00C0
#define REG_CHIP_ID_VAL  0x8870  /* Expected chip ID */

/* MAC Address Registers */
#define REG_MAC_ADDR_0   0x0010  /* MAC Address Low (bytes 0-1) */
#define REG_MAC_ADDR_2   0x0012  /* MAC Address Mid (bytes 2-3) */
#define REG_MAC_ADDR_4   0x0014  /* MAC Address High (bytes 4-5) */

/* Transmit Control Registers */
#define REG_TX_CTRL      0x0070
#define TX_CTRL_TXCE     0x0001  /* TX Checksum Enable */
#define TX_CTRL_TXE      0x0004  /* TX Enable */
#define TX_CTRL_TXFCE    0x0008  /* TX Flow Control Enable */
#define TX_CTRL_TXPE     0x0010  /* TX Padding Enable */

/* Receive Control Registers */
#define REG_RX_CTRL1     0x0074
#define REG_RX_CTRL2     0x0076
#define RX_CTRL1_RXE     0x0001  /* RX Enable */
#define RX_CTRL1_RXFCE   0x0008  /* RX Flow Control Enable */
#define RX_CTRL1_RXBE    0x0010  /* RX Broadcast Enable */
#define RX_CTRL1_RXME    0x0020  /* RX Multicast Enable */
#define RX_CTRL1_RXUE    0x0040  /* RX Unicast Enable */
#define RX_CTRL1_RXAE    0x0080  /* RX All Enable */
#define RX_CTRL1_RXCE    0x0100  /* RX Checksum Enable */

/* Interrupt Registers */
#define REG_INT_STATUS   0x009C
#define REG_INT_ENABLE   0x009A
#define INT_LINKC        0x8000  /* Link Change */
#define INT_TXIE         0x4000  /* TX Interrupt Enable */
#define INT_RXIE         0x2000  /* RX Interrupt Enable */
#define INT_RXOIE        0x0800  /* RX Overrun Interrupt Enable */
#define INT_TXPSIE       0x0200  /* TX Process Stopped Interrupt Enable */
#define INT_RXPSIE       0x0100  /* RX Process Stopped Interrupt Enable */
#define INT_TXSAIE       0x0040  /* TX Space Available Interrupt Enable */
#define INT_RXWFDIE      0x0020  /* RX Wakeup Frame Detect Interrupt Enable */
#define INT_RXMPDIE      0x0010  /* RX Magic Packet Detect Interrupt Enable */
#define INT_LDIE         0x0008  /* Link Down Interrupt Enable */
#define INT_EDIE         0x0004  /* Energy Detect Interrupt Enable */
#define INT_SPIBEI       0x0002  /* SPI Bus Error Interrupt */

/* Frame Header Control */
#define REG_RXQ_CMD      0x0082
#define REG_TXQ_CMD      0x0080
#define RXQ_CMD_CNTL     0x0001
#define RXQ_CMD_START    0x0008
#define TXQ_CMD_ENQUEUE  0x0001

/* Frame Data Pointer */
#define REG_RX_FDP       0x0086
#define REG_TX_FDP       0x0084

/* QMU Packet Length */
#define REG_RX_FHR       0x0088
#define REG_TX_FHR       0x008A

/* Frame Count and Threshold */
#define REG_RX_FCTFC     0x008C
#define REG_TX_FCTFC     0x008E

/* PHY Special Control/Status */
#define REG_PHY_CNTL     0x00F2
#define PHY_AUTO_NEG_EN  0x1000  /* Auto-negotiation Enable */
#define PHY_RESTART_AN   0x0200  /* Restart Auto-negotiation */
#define PHY_FORCE_100    0x2000  /* Force 100Mbps */
#define PHY_FORCE_FDX    0x0100  /* Force Full Duplex */

/* Reset Control */
#define REG_RESET_CTRL   0x0026
#define GLOBAL_SOFT_RESET 0x0001

/* Power Management */
#define REG_POWER_CNTL   0x00DD
#define PWR_MGMT_MODE_MASK 0x0018
#define PWR_MGMT_NORMAL   0x0000
#define PWR_MGMT_ENERGY   0x0008
#define PWR_MGMT_SOFT_PD  0x0010

/* Buffer memory */
#define KSZ8851SNL_TX_MEM_SIZE  0x1800  /* 6KB TX memory */
#define KSZ8851SNL_RX_MEM_SIZE  0x3000  /* 12KB RX memory */

/* Frame overhead */
#define FRAME_HEADER_LEN    8    /* Frame header length */
#define FRAME_FOOTER_LEN    4    /* Frame footer/status length */

/* Microchip's OUI */
#define MICROCHIP_OUI_B0 0x00
#define MICROCHIP_OUI_B1 0x04
#define MICROCHIP_OUI_B2 0xA3

struct eth_ksz8851snl_config {
	struct spi_dt_spec spi;
	struct gpio_dt_spec interrupt;
	struct gpio_dt_spec reset;
	uint8_t full_duplex;
	int32_t timeout;
	bool random_mac;
};

struct eth_ksz8851snl_runtime {
	struct net_if *iface;
	K_KERNEL_STACK_MEMBER(thread_stack,
			      CONFIG_ETH_KSZ8851SNL_RX_THREAD_STACK_SIZE);
	struct k_thread thread;
	uint8_t mac_address[6];
	struct gpio_callback gpio_cb;
	struct k_sem tx_rx_sem;
	struct k_sem int_sem;
	bool iface_initialized : 1;
	bool iface_carrier_on_init : 1;
};

#endif /*_KSZ8851SNL_*/