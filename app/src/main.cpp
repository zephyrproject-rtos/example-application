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
#include "wifiManager.hpp"


wifiManager wifi;

int main(void)
{
    /* Main Function */
    MYLOG("Hello World!");

    /* Prechecks for Connecting with Wifi */
    // struct wifi_iface_status status = {0};

    // status = get_wifi_status(iface);

    // if (status.state == WIFI_STATE_INACTIVE || status.state == WIFI_STATE_DISCONNECTED)
    // {
    //     MYLOG("WiFi is Inactive or Disconnected");
    // }
    // else
    // {
    //     while (status.state != WIFI_STATE_COMPLETED)
    //     {
    //         if ((status.state == WIFI_STATE_INACTIVE) || (status.state == WIFI_STATE_DISCONNECTED))
    //         {
    //             break;
    //         }

    //         status = get_wifi_status(iface);
    //     }
    // }

    /* Now Connect with Wifi */
    // wifi_connect();
    // MYLOG("Waiting for Wifi to connect");

    // do
    // {
    //     status = get_wifi_status(iface);

    //     if (status.state >= WIFI_STATE_ASSOCIATED)
    //     {
    //         break;
    //     }

    // }while(1);

    // MYLOG("Checking Wifi Status");

    // wifi_status();

    // if (get_wifi_status(iface).state == WIFI_STATE_ASSOCIATED)
    // {
    //     MYLOG("Wifi Connected");
    // }
    // else
    // {
    //     MYLOG("Wifi Not Connected. State: %d", status.state);
    // }

    /* Check for IP address assignment */
    // struct net_if_ipv4 *ipv4 = net_if_get_config(iface)->ip.ipv4;

    // if (ipv4 && ipv4->unicast[0].ipv4.is_used)
    // {
    //     struct in_addr addr = ipv4->unicast[0].ipv4.address.in_addr;
    //     char ip_str[NET_IPV4_ADDR_LEN];

    //     net_addr_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));
    //     MYLOG("✅ IP address assigned: %s", ip_str);
    // }
    // else
    // {
    //     MYLOG("❌ No IPv4 address assigned yet");

    //     /* Wait for the handler to be called */
    //     int64_t start = k_uptime_get();
    //     // ipv4_sem_take();

    //     /* wait for ip address to be assigned */
    //     MYLOG("⏰ Time to wait for IP Address to be assigned: %lld ms",(k_uptime_get() - start));

    // }

    // MYLOG("Disconnecting WIFI");
    // wifi_disconnect();
    // MYLOG("WIFI Disconnected");

    wifi.init();
    k_busy_wait(20000000);

    wifi.connect();
    k_busy_wait(20000000);

    while (true)
    {
        wifi.tick();
        k_busy_wait(20000000);
    }

    return 0;
}
