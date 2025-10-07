/* KSZ8851SNL Stand-alone Ethernet Controller with SPI
 *
 * Copyright (c) 2024 Zephyr Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT microchip_ksz8851snl

#define LOG_MODULE_NAME eth_ksz8851snl
#define LOG_LEVEL CONFIG_ETHERNET_LOG_LEVEL

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <string.h>
#include <errno.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/net/net_pkt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/ethernet.h>
#include <ethernet/eth_stats.h>

#include "eth_ksz8851snl_priv.h"

static int ksz8851snl_spi_read_reg(const struct device *dev, uint16_t reg_addr,
				   uint16_t *data)
{
	const struct eth_ksz8851snl_config *config = dev->config;
	uint16_t cmd = CMD_READ | ((reg_addr << ADDR_SHIFT) & (ADDR_MASK << ADDR_SHIFT)) | BE1 | BE0;
	uint8_t tx_buf[4] = {cmd >> 8, cmd & 0xFF, 0, 0};
	uint8_t rx_buf[4];
	
	const struct spi_buf tx_spi_buf = {
		.buf = tx_buf,
		.len = sizeof(tx_buf)
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_spi_buf,
		.count = 1
	};
	const struct spi_buf rx_spi_buf = {
		.buf = rx_buf,
		.len = sizeof(rx_buf)
	};
	const struct spi_buf_set rx = {
		.buffers = &rx_spi_buf,
		.count = 1
	};

	int ret = spi_transceive_dt(&config->spi, &tx, &rx);
	if (ret == 0) {
		*data = (rx_buf[2] << 8) | rx_buf[3];
	}
	return ret;
}

static int ksz8851snl_spi_write_reg(const struct device *dev, uint16_t reg_addr,
				    uint16_t data)
{
	const struct eth_ksz8851snl_config *config = dev->config;
	uint16_t cmd = CMD_WRITE | ((reg_addr << ADDR_SHIFT) & (ADDR_MASK << ADDR_SHIFT)) | BE1 | BE0;
	uint8_t tx_buf[4] = {cmd >> 8, cmd & 0xFF, data >> 8, data & 0xFF};
	
	const struct spi_buf tx_spi_buf = {
		.buf = tx_buf,
		.len = sizeof(tx_buf)
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_spi_buf,
		.count = 1
	};

	return spi_write_dt(&config->spi, &tx);
}

static int ksz8851snl_read_fifo(const struct device *dev, uint8_t *data, size_t len)
{
	const struct eth_ksz8851snl_config *config = dev->config;
	uint16_t cmd = CMD_READ | ((REG_RX_FDP << ADDR_SHIFT) & (ADDR_MASK << ADDR_SHIFT)) | BE3 | BE2 | BE1 | BE0;
	uint8_t cmd_buf[2] = {cmd >> 8, cmd & 0xFF};
	
	const struct spi_buf tx_buf = {
		.buf = cmd_buf,
		.len = sizeof(cmd_buf)
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1
	};
	const struct spi_buf rx_bufs[2] = {
		{.buf = NULL, .len = 2}, /* Skip command echo */
		{.buf = data, .len = len}
	};
	const struct spi_buf_set rx = {
		.buffers = rx_bufs,
		.count = 2
	};

	return spi_transceive_dt(&config->spi, &tx, &rx);
}

static int ksz8851snl_write_fifo(const struct device *dev, const uint8_t *data, size_t len)
{
	const struct eth_ksz8851snl_config *config = dev->config;
	uint16_t cmd = CMD_WRITE | ((REG_TX_FDP << ADDR_SHIFT) & (ADDR_MASK << ADDR_SHIFT)) | BE3 | BE2 | BE1 | BE0;
	uint8_t cmd_buf[2] = {cmd >> 8, cmd & 0xFF};
	
	const struct spi_buf tx_bufs[2] = {
		{.buf = cmd_buf, .len = sizeof(cmd_buf)},
		{.buf = (void *)data, .len = len}
	};
	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = 2
	};

	return spi_write_dt(&config->spi, &tx);
}

static int ksz8851snl_soft_reset(const struct device *dev)
{
	int ret;
	uint16_t data;

	ret = ksz8851snl_spi_write_reg(dev, REG_RESET_CTRL, GLOBAL_SOFT_RESET);
	if (ret) {
		return ret;
	}

	k_msleep(10);

	/* Wait for reset to complete */
	do {
		ret = ksz8851snl_spi_read_reg(dev, REG_CHIP_ID, &data);
		if (ret) {
			return ret;
		}
		k_msleep(1);
	} while (data != REG_CHIP_ID_VAL);

	LOG_DBG("Soft reset completed, chip ID: 0x%04X", data);
	return 0;
}

static int ksz8851snl_verify_chip_id(const struct device *dev)
{
	uint16_t chip_id;
	int ret;

	ret = ksz8851snl_spi_read_reg(dev, REG_CHIP_ID, &chip_id);
	if (ret) {
		LOG_ERR("Failed to read chip ID: %d", ret);
		return ret;
	}

	if (chip_id != REG_CHIP_ID_VAL) {
		LOG_ERR("Invalid chip ID: 0x%04X, expected 0x%04X", chip_id, REG_CHIP_ID_VAL);
		return -ENODEV;
	}

	LOG_INF("KSZ8851SNL chip ID verified: 0x%04X", chip_id);
	return 0;
}

static int ksz8851snl_set_mac_address(const struct device *dev, const uint8_t *mac_addr)
{
	int ret;
	uint16_t addr_word;

	addr_word = (mac_addr[1] << 8) | mac_addr[0];
	ret = ksz8851snl_spi_write_reg(dev, REG_MAC_ADDR_0, addr_word);
	if (ret) return ret;

	addr_word = (mac_addr[3] << 8) | mac_addr[2];
	ret = ksz8851snl_spi_write_reg(dev, REG_MAC_ADDR_2, addr_word);
	if (ret) return ret;

	addr_word = (mac_addr[5] << 8) | mac_addr[4];
	ret = ksz8851snl_spi_write_reg(dev, REG_MAC_ADDR_4, addr_word);

	if (ret == 0) {
		LOG_INF("MAC address set: %02x:%02x:%02x:%02x:%02x:%02x",
			mac_addr[0], mac_addr[1], mac_addr[2],
			mac_addr[3], mac_addr[4], mac_addr[5]);
	}

	return ret;
}

static int ksz8851snl_init_phy(const struct device *dev)
{
	const struct eth_ksz8851snl_config *config = dev->config;
	uint16_t phy_ctrl = PHY_AUTO_NEG_EN;
	int ret;

	if (config->full_duplex) {
		phy_ctrl |= PHY_FORCE_FDX | PHY_FORCE_100;
	}

	ret = ksz8851snl_spi_write_reg(dev, REG_PHY_CNTL, phy_ctrl);
	if (ret == 0) {
		LOG_DBG("PHY initialized with control: 0x%04X", phy_ctrl);
	}

	return ret;
}

static void ksz8851snl_gpio_callback(const struct device *dev,
				     struct gpio_callback *cb, uint32_t pins)
{
	struct eth_ksz8851snl_runtime *context =
		CONTAINER_OF(cb, struct eth_ksz8851snl_runtime, gpio_cb);

	k_sem_give(&context->int_sem);
}

static int ksz8851snl_tx(const struct device *dev, struct net_pkt *pkt)
{
	const struct eth_ksz8851snl_config *config = dev->config;
	struct eth_ksz8851snl_runtime *context = dev->data;
	struct net_buf *frag;
	uint16_t frame_len = net_pkt_get_len(pkt);
	uint16_t tx_space;
	uint32_t frame_header;
	int ret;

	if (frame_len > NET_ETH_MAX_FRAME_SIZE) {
		LOG_ERR("Frame too large: %u bytes", frame_len);
		return -EMSGSIZE;
	}

	k_sem_take(&context->tx_rx_sem, K_FOREVER);

	/* Check available TX memory space */
	ret = ksz8851snl_spi_read_reg(dev, REG_TX_FCTFC, &tx_space);
	if (ret || tx_space < (frame_len + FRAME_HEADER_LEN + 3) / 4) {
		LOG_ERR("Insufficient TX space");
		k_sem_give(&context->tx_rx_sem);
		return -ENOMEM;
	}

	/* Write frame header */
	frame_header = frame_len | (frame_len << 16);
	ret = ksz8851snl_write_fifo(dev, (uint8_t *)&frame_header, 4);
	if (ret) {
		LOG_ERR("Failed to write frame header");
		k_sem_give(&context->tx_rx_sem);
		return ret;
	}

	/* Write frame data */
	for (frag = pkt->frags; frag; frag = frag->frags) {
		ret = ksz8851snl_write_fifo(dev, frag->data, frag->len);
		if (ret) {
			LOG_ERR("Failed to write frame data");
			k_sem_give(&context->tx_rx_sem);
			return ret;
		}
	}

	/* Pad frame if necessary */
	if (frame_len < 60) {
		uint8_t padding[60] = {0};
		ret = ksz8851snl_write_fifo(dev, padding, 60 - frame_len);
		if (ret) {
			LOG_ERR("Failed to write padding");
			k_sem_give(&context->tx_rx_sem);
			return ret;
		}
	}

	/* Enqueue frame for transmission */
	ret = ksz8851snl_spi_write_reg(dev, REG_TXQ_CMD, TXQ_CMD_ENQUEUE);
	if (ret) {
		LOG_ERR("Failed to enqueue TX frame");
		k_sem_give(&context->tx_rx_sem);
		return ret;
	}

	k_sem_give(&context->tx_rx_sem);

	LOG_DBG("TX frame queued, length: %u", frame_len);
	return 0;
}

static void ksz8851snl_read_packet(const struct device *dev)
{
	const struct eth_ksz8851snl_config *config = dev->config;
	struct eth_ksz8851snl_runtime *context = dev->data;
	struct net_pkt *pkt;
	struct net_buf *pkt_buf;
	uint16_t frame_hdr[2];
	uint16_t frame_len, status;
	int ret;

	/* Read frame header */
	ret = ksz8851snl_read_fifo(dev, (uint8_t *)frame_hdr, 4);
	if (ret) {
		LOG_ERR("Failed to read frame header");
		return;
	}

	status = frame_hdr[0];
	frame_len = frame_hdr[1] & 0x07FF;

	/* Check frame validity */
	if (frame_len < 14 || frame_len > NET_ETH_MAX_FRAME_SIZE) {
		LOG_ERR("Invalid frame length: %u", frame_len);
		return;
	}

	/* Allocate packet */
	pkt = net_pkt_rx_alloc_with_buffer(context->iface, frame_len,
					   AF_UNSPEC, 0, K_MSEC(config->timeout));
	if (!pkt) {
		LOG_ERR("Could not allocate rx buffer");
		eth_stats_update_errors_rx(context->iface);
		return;
	}

	pkt_buf = pkt->buffer;

	/* Read frame data */
	do {
		size_t frag_len = net_buf_tailroom(pkt_buf);
		size_t read_len = MIN(frame_len, frag_len);

		ret = ksz8851snl_read_fifo(dev, pkt_buf->data + pkt_buf->len, read_len);
		if (ret) {
			LOG_ERR("Failed to read frame data");
			net_pkt_unref(pkt);
			return;
		}

		net_buf_add(pkt_buf, read_len);
		frame_len -= read_len;
		if (frame_len > 0) {
			pkt_buf = pkt_buf->frags;
		}
	} while (frame_len > 0);

	/* Submit packet to network stack */
	ret = net_recv_data(context->iface, pkt);
	if (ret < 0) {
		LOG_ERR("Failed to submit packet to network stack");
		net_pkt_unref(pkt);
	} else {
		LOG_DBG("RX packet submitted, length: %u", net_pkt_get_len(pkt));
	}
}

static int ksz8851snl_rx(const struct device *dev)
{
	struct eth_ksz8851snl_runtime *context = dev->data;
	uint16_t frame_count;
	int ret;

	k_sem_take(&context->tx_rx_sem, K_FOREVER);

	/* Check if frames are available */
	ret = ksz8851snl_spi_read_reg(dev, REG_RX_FCTFC, &frame_count);
	if (ret || (frame_count >> 8) == 0) {
		k_sem_give(&context->tx_rx_sem);
		return ret;
	}

	while ((frame_count >> 8) > 0) {
		/* Start RX frame read */
		ret = ksz8851snl_spi_write_reg(dev, REG_RXQ_CMD, RXQ_CMD_CNTL);
		if (ret) {
			break;
		}

		ksz8851snl_read_packet(dev);

		/* Check for more frames */
		ret = ksz8851snl_spi_read_reg(dev, REG_RX_FCTFC, &frame_count);
		if (ret) {
			break;
		}
	}

	k_sem_give(&context->tx_rx_sem);
	return ret;
}

static void ksz8851snl_rx_thread(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	const struct device *dev = p1;
	struct eth_ksz8851snl_runtime *context = dev->data;
	uint16_t int_status, int_enable;
	int ret;

	while (true) {
		k_sem_take(&context->int_sem, K_FOREVER);

		/* Disable interrupts during processing */
		ret = ksz8851snl_spi_read_reg(dev, REG_INT_ENABLE, &int_enable);
		if (ret) {
			continue;
		}
		
		ksz8851snl_spi_write_reg(dev, REG_INT_ENABLE, 0);

		/* Read and clear interrupt status */
		ret = ksz8851snl_spi_read_reg(dev, REG_INT_STATUS, &int_status);
		if (ret) {
			ksz8851snl_spi_write_reg(dev, REG_INT_ENABLE, int_enable);
			continue;
		}

		ksz8851snl_spi_write_reg(dev, REG_INT_STATUS, int_status);

		/* Handle link change */
		if (int_status & INT_LINKC) {
			LOG_INF("Link status changed");
			/* TODO: Update carrier status based on link state */
		}

		/* Handle RX */
		if (int_status & INT_RXIE) {
			ksz8851snl_rx(dev);
		}

		/* Re-enable interrupts */
		ksz8851snl_spi_write_reg(dev, REG_INT_ENABLE, int_enable);
	}
}

static enum ethernet_hw_caps ksz8851snl_get_capabilities(const struct device *dev)
{
	ARG_UNUSED(dev);

	return ETHERNET_LINK_10BASE_T | ETHERNET_LINK_100BASE_T
#if defined(CONFIG_NET_VLAN)
		| ETHERNET_HW_VLAN
#endif
		;
}

static int ksz8851snl_set_config(const struct device *dev,
				 enum ethernet_config_type type,
				 const struct ethernet_config *config)
{
	struct eth_ksz8851snl_runtime *context = dev->data;

	if (type == ETHERNET_CONFIG_TYPE_MAC_ADDRESS) {
		memcpy(context->mac_address, config->mac_address.addr,
		       sizeof(context->mac_address));
		
		ksz8851snl_set_mac_address(dev, context->mac_address);
		
		if (context->iface != NULL) {
			net_if_set_link_addr(context->iface, context->mac_address,
					     sizeof(context->mac_address),
					     NET_LINK_ETHERNET);
		}

		return 0;
	}

	return -ENOTSUP;
}

static void ksz8851snl_iface_init(struct net_if *iface)
{
	const struct device *dev = net_if_get_device(iface);
	struct eth_ksz8851snl_runtime *context = dev->data;

	net_if_set_link_addr(iface, context->mac_address,
			     sizeof(context->mac_address),
			     NET_LINK_ETHERNET);

	context->iface = iface;
	context->iface_initialized = true;

	ethernet_init(iface);

	if (context->iface_carrier_on_init) {
		net_eth_carrier_on(iface);
		context->iface_carrier_on_init = false;
	}
}

static const struct ethernet_api ksz8851snl_api = {
	.iface_api.init = ksz8851snl_iface_init,
	.get_capabilities = ksz8851snl_get_capabilities,
	.set_config = ksz8851snl_set_config,
	.send = ksz8851snl_tx,
};

static int ksz8851snl_init(const struct device *dev)
{
	const struct eth_ksz8851snl_config *config = dev->config;
	struct eth_ksz8851snl_runtime *context = dev->data;
	int ret;

	/* Initialize semaphores */
	k_sem_init(&context->tx_rx_sem, 1, 1);
	k_sem_init(&context->int_sem, 0, 1);

	/* Check SPI device readiness */
	if (!spi_is_ready_dt(&config->spi)) {
		LOG_ERR("SPI device not ready");
		return -ENODEV;
	}

	/* Hardware reset if available */
	if (config->reset.port) {
		if (!gpio_is_ready_dt(&config->reset)) {
			LOG_ERR("Reset GPIO device not ready");
			return -ENODEV;
		}

		ret = gpio_pin_configure_dt(&config->reset, GPIO_OUTPUT_ACTIVE);
		if (ret) {
			LOG_ERR("Failed to configure reset pin");
			return ret;
		}

		k_msleep(1);
		gpio_pin_set_dt(&config->reset, 0);
		k_msleep(10);
	}

	/* Perform software reset and verify chip ID */
	ret = ksz8851snl_soft_reset(dev);
	if (ret) {
		LOG_ERR("Soft reset failed");
		return ret;
	}

	ret = ksz8851snl_verify_chip_id(dev);
	if (ret) {
		return ret;
	}

	/* Generate or use configured MAC address */
	if (config->random_mac) {
		gen_random_mac(context->mac_address,
			       MICROCHIP_OUI_B0, MICROCHIP_OUI_B1, MICROCHIP_OUI_B2);
		LOG_INF("Generated random MAC address");
	}

	ret = ksz8851snl_set_mac_address(dev, context->mac_address);
	if (ret) {
		LOG_ERR("Failed to set MAC address");
		return ret;
	}

	/* Initialize PHY */
	ret = ksz8851snl_init_phy(dev);
	if (ret) {
		LOG_ERR("Failed to initialize PHY");
		return ret;
	}

	/* Configure transmit control */
	ret = ksz8851snl_spi_write_reg(dev, REG_TX_CTRL, 
					TX_CTRL_TXCE | TX_CTRL_TXE | TX_CTRL_TXPE);
	if (ret) {
		LOG_ERR("Failed to configure TX control");
		return ret;
	}

	/* Configure receive control */
	ret = ksz8851snl_spi_write_reg(dev, REG_RX_CTRL1, 
					RX_CTRL1_RXE | RX_CTRL1_RXBE | RX_CTRL1_RXME | 
					RX_CTRL1_RXUE | RX_CTRL1_RXCE);
	if (ret) {
		LOG_ERR("Failed to configure RX control");
		return ret;
	}

	/* Configure interrupt GPIO */
	if (!gpio_is_ready_dt(&config->interrupt)) {
		LOG_ERR("Interrupt GPIO device not ready");
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&config->interrupt, GPIO_INPUT);
	if (ret) {
		LOG_ERR("Failed to configure interrupt pin");
		return ret;
	}

	gpio_init_callback(&context->gpio_cb, ksz8851snl_gpio_callback,
			   BIT(config->interrupt.pin));

	ret = gpio_add_callback(config->interrupt.port, &context->gpio_cb);
	if (ret) {
		LOG_ERR("Failed to add interrupt callback");
		return ret;
	}

	ret = gpio_pin_interrupt_configure_dt(&config->interrupt,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret) {
		LOG_ERR("Failed to configure interrupt");
		return ret;
	}

	/* Enable interrupts */
	ret = ksz8851snl_spi_write_reg(dev, REG_INT_ENABLE, 
					INT_LINKC | INT_RXIE | INT_TXIE);
	if (ret) {
		LOG_ERR("Failed to enable interrupts");
		return ret;
	}

	/* Start RX thread */
	k_thread_create(&context->thread, context->thread_stack,
			K_KERNEL_STACK_SIZEOF(context->thread_stack),
			ksz8851snl_rx_thread, (void *)dev, NULL, NULL,
			K_PRIO_COOP(CONFIG_ETH_KSZ8851SNL_RX_THREAD_PRIO),
			0, K_NO_WAIT);

	k_thread_name_set(&context->thread, "ksz8851snl_rx");

	LOG_INF("KSZ8851SNL initialized successfully");
	return 0;
}

#define ETH_KSZ8851SNL_DEFINE(inst)						\
	static const struct eth_ksz8851snl_config eth_ksz8851snl_config_##inst = { \
		.spi = SPI_DT_SPEC_INST_GET(inst, SPI_OP_MODE_MASTER |	\
					    SPI_WORD_SET(8), 0),		\
		.interrupt = GPIO_DT_SPEC_INST_GET(inst, int_gpios),	\
		.reset = GPIO_DT_SPEC_INST_GET_OR(inst, reset_gpios, {0}), \
		.full_duplex = DT_INST_PROP_OR(inst, full_duplex, false), \
		.timeout = CONFIG_ETH_KSZ8851SNL_TIMEOUT,		\
		.random_mac = !NODE_HAS_VALID_MAC_ADDR(DT_DRV_INST(inst)), \
	};									\
										\
	static struct eth_ksz8851snl_runtime eth_ksz8851snl_runtime_##inst = { \
		.mac_address = DT_INST_PROP_OR(inst, local_mac_address, {0}), \
	};									\
										\
	ETH_NET_DEVICE_DT_INST_DEFINE(inst, ksz8851snl_init, NULL,	\
				      &eth_ksz8851snl_runtime_##inst,	\
				      &eth_ksz8851snl_config_##inst,	\
				      CONFIG_ETH_INIT_PRIORITY,		\
				      &ksz8851snl_api,			\
				      NET_ETH_MTU);

DT_INST_FOREACH_STATUS_OKAY(ETH_KSZ8851SNL_DEFINE)