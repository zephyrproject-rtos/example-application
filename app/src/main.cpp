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

    /* Now Connect with Wifi */
    wifi.connect();
    MYLOG("Waiting for Wifi to connect");

    while (true)
    {
        wifi.tick();
    }

    return 0;
}
