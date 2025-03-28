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
    struct wifi_iface_status status = {0};

    wifi.init();

    wifi.tick();
    int64_t start = k_uptime_get();

    bool isConnectRequested = false;
    wifiStateEnum state;

    while (true)
    {
        wifi.tick();
        state = wifi.getWifiState();

        if (wifiStateEnum::IDLE == state)
        {
            /* If 500ms have passed we can connect using Wifi */
            if ((k_uptime_get() - start > 500) && !isConnectRequested)
            {
                MYLOG("Waiting for Wifi to connect");
                wifi.connect();
                isConnectRequested = true;
            }
        }
        else if (wifiStateEnum::CONNECTING == state)
        {
            /* Wait for Connection to be established */
        }
        else if (wifiStateEnum::CONNECTED == state)
        {
            /* Do WiFI Stuff after this */
            isConnectRequested = false;
        }
        else if (wifiStateEnum::ERROR == state)
        {
            if (k_uptime_get() - start > 10000)
            {
                MYLOG("❌ Error in Wifi Initialization. ReInitializing");
                wifi.connect();
            }
        }
    }

    return 0;
}
