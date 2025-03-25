/*
 * Copyright (c) 2025 Osama Salah-ud-Din.
*/

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>

#include "myLogger.h"
#include "handlers.h"
#include "wifi.h"

static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback ipv4_cb;

void register_wifi_events()
{
    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler,
                                    NET_EVENT_WIFI_CONNECT_RESULT |
                                    NET_EVENT_WIFI_DISCONNECT_RESULT |
                                    NET_EVENT_WIFI_SCAN_RESULT );

    net_mgmt_init_event_callback(&ipv4_cb, ipv4_mgmt_event_handler, NET_EVENT_IPV4_ADDR_ADD);

    net_mgmt_add_event_callback(&wifi_cb);
    net_mgmt_add_event_callback(&ipv4_cb);
}

static void ip_addr_callback(struct net_if *iface, struct net_if_addr *addr, void *user_data)
{
    char ip_str[NET_IPV4_ADDR_LEN];

    if (addr->addr_type == NET_ADDR_DHCP || addr->addr_type == NET_ADDR_MANUAL) {
        net_addr_ntop(AF_INET, &addr->address.in_addr, ip_str, sizeof(ip_str));
        printk("Assigned IPv4 address: %s\n", ip_str);
    }
}

void check_ipv4_address(void)
{
    struct net_if *iface = net_if_get_default();

    if (!iface)
    {
        printk("No default network interface found.\n");
        return;
    }

    net_if_ipv4_addr_foreach(iface, ip_addr_callback, NULL);
}

int main(void)
{
    register_wifi_events();

    struct net_if *iface = net_if_get_default();

    /* Main Function */
    MYLOG("Hello World!\n");

    if (iface)
    {
        net_if_up(iface);
        MYLOG("Network interface found!");
    }
    else
    {
        LOG_ERR("No network interface found!\n");
    }

    /* Prechecks for Connecting with Wifi */
    struct wifi_iface_status status = {0};

    status = get_wifi_status(iface);

    if (status.state == WIFI_STATE_INACTIVE || status.state == WIFI_STATE_DISCONNECTED)
    {
        MYLOG("WiFi is Inactive or Disconnected");
    }
    else
    {
        while (status.state != WIFI_STATE_COMPLETED)
        {
            if ((status.state == WIFI_STATE_INACTIVE) || (status.state == WIFI_STATE_DISCONNECTED))
            {
                break;
            }

            status = get_wifi_status(iface);
        }
    }

    /* Now Connect with Wifi */
    wifi_connect();
    MYLOG("Waiting for Wifi to connect");

    do
    {
        status = get_wifi_status(iface);

        if (status.state >= WIFI_STATE_ASSOCIATED)
        {
            break;
        }

    }while(1);

    MYLOG("Checking Wifi Status");

    wifi_status();

    if (get_wifi_status(iface).state == WIFI_STATE_ASSOCIATED)
    {
        MYLOG("Wifi Connected");
    }
    else
    {
        MYLOG("Wifi Not Connected. State: %d", status.state);
    }

    /* Check for IP address assignment */
    struct net_if_ipv4 *ipv4 = net_if_get_config(iface)->ip.ipv4;

    if (ipv4 && ipv4->unicast[0].ipv4.is_used)
    {
        struct in_addr addr = ipv4->unicast[0].ipv4.address.in_addr;
        char ip_str[NET_IPV4_ADDR_LEN];

        net_addr_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));
        MYLOG("✅ IP address assigned: %s", ip_str);
    }
    else
    {
        MYLOG("❌ No IPv4 address assigned yet");

        /* Wait for the handler to be called */
        int64_t start = k_uptime_get();
        ipv4_sem_take();

        /* wait for ip address to be assigned */
        MYLOG("⏰ Time to wait for IP Address to be assigned: %lld ms",(k_uptime_get() - start));

    }


    // // Ping Google DNS 4 times
    // ping("8.8.8.8", 4);


    // printk("\nConnecting to HTTP Server:\n");
    // sock = connect_socket(&res, 80);
    // http_get(sock, "iot.beyondlogic.org", "/LoremIpsum.txt");
    // zsock_close(sock);

    // Stay connected for 30 seconds, then disconnect.
    // k_sleep(K_SECONDS(30));

    MYLOG("Disconnecting WIFI");
    wifi_disconnect();
    MYLOG("WIFI Disconnected");

    return 0;
}
