/*
 * Copyright (c) 2024 Zephyr Project
 * SPDX-License-Identifier: Apache-2.0
 *
 * KSZ8851SNL Ethernet Example Application
 */

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(eth_example, CONFIG_APP_LOG_LEVEL);

static struct net_mgmt_event_callback mgmt_cb;

static void event_handler(struct net_mgmt_event_callback *cb,
			  uint32_t mgmt_event, struct net_if *iface)
{
	if (mgmt_event == NET_EVENT_IPV4_ADDR_ADD) {
		char addr_str[NET_IPV4_ADDR_LEN];
		const struct in_addr *addr;

		addr = &iface->config.ip.ipv4->unicast[0].address.in_addr;

		if (net_addr_ntop(AF_INET, addr, addr_str, sizeof(addr_str))) {
			LOG_INF("IPv4 address: %s", addr_str);
			LOG_INF("Lease time: %u seconds",
				iface->config.dhcpv4.lease_time);
		}
	} else if (mgmt_event == NET_EVENT_IPV4_DHCP_BOUND) {
		LOG_INF("DHCP bound");
	} else if (mgmt_event == NET_EVENT_IPV4_DHCP_START) {
		LOG_INF("DHCP started");
	}
}

void ethernet_example_init(void)
{
	struct net_if *iface;
	struct ethernet_context *eth_ctx;

	LOG_INF("Initializing Ethernet example");

	/* Get default network interface (Ethernet) */
	iface = net_if_get_default();
	if (!iface) {
		LOG_ERR("No network interface found");
		return;
	}

	/* Check if it's an Ethernet interface */
	if (net_if_l2(iface) != &NET_L2_GET_NAME(ETHERNET)) {
		LOG_ERR("Default interface is not Ethernet");
		return;
	}

	LOG_INF("Network interface: %s", net_if_get_device(iface)->name);

	/* Print MAC address */
	const uint8_t *mac = net_if_get_link_addr(iface)->addr;
	LOG_INF("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x",
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	/* Setup network management event callback for IP address changes */
	net_mgmt_init_event_callback(&mgmt_cb, event_handler,
				     NET_EVENT_IPV4_ADDR_ADD |
				     NET_EVENT_IPV4_DHCP_BOUND |
				     NET_EVENT_IPV4_DHCP_START);
	net_mgmt_add_event_callback(&mgmt_cb);

	/* Start DHCP */
	LOG_INF("Starting DHCP client...");
	net_dhcpv4_start(iface);

	LOG_INF("Ethernet initialized successfully");
}
