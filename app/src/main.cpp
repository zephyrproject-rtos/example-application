/*
 * Copyright (c) 2025 Osama Salah-ud-Din.
*/

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <iostream>
#include <string>
#include <app_version.h>

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>

const std::string CONFIG_WIFI_SSID(WIFI_SSID);
const std::string CONFIG_WIFI_PASSWORD(WIFI_PASSWORD);

void wifi_connect()
{
    struct net_if *iface = net_if_get_default();

    struct wifi_connect_req_params params =
    {
        .ssid = (const uint8_t*) (CONFIG_WIFI_SSID.c_str()),
        .ssid_length = CONFIG_WIFI_SSID.length(),
        .psk = (const uint8_t*) (CONFIG_WIFI_PASSWORD.c_str()),
        .psk_length = CONFIG_WIFI_PASSWORD.length(),
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params)))
    {
        LOG_ERR("Failed to connect to WiFi network!");
    }
    else
    {
        LOG_INF("Connected to WiFi network!");
    }
}


int main(void)
{
    struct net_if *iface = net_if_get_default();

    /* Main Function */
    printk("Hello World!\n");

    if (iface)
    {
        net_if_up(iface);
        LOG_INF("Network interface found!\n");
    }
    else
    {
        LOG_ERR("No network interface found!\n");
    }

    wifi_connect();

    return 0;
}
