#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>

#include "myLogger.h"
#include "wifi.h"

const std::string CONFIG_WIFI_SSID(WIFI_SSID);
const std::string CONFIG_WIFI_PASSWORD(WIFI_PASSWORD);

// static K_SEM_DEFINE(ipv4_address_obtained, 0, 1);

void wifi_connect(void)
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

    MYLOG("Connecting to the Wifi Network [SSID]: %s", CONFIG_WIFI_SSID.c_str());

    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
    if (ret)
    {
        MYLOG("Failed to connect to WiFi network! [Error]:%d", ret);
    }
}

wifi_iface_status get_wifi_status(struct net_if* iface)
{
    struct wifi_iface_status status = {0};

    int ret = net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, iface, &status, sizeof(struct wifi_iface_status));
    if (ret)
    {
        MYLOG("WiFi Status Request Failed");
        return status;
    }

    return status;
}

void wifi_status(void)
{
    struct net_if *iface = net_if_get_default();

    struct wifi_iface_status status = {0};

    status = get_wifi_status(iface);

    MYLOG("Wifi Status: %d\n", status.state);

    if (status.state >= WIFI_STATE_ASSOCIATED)
    {
        MYLOG("[WIFI]SSID: %-32s", status.ssid);
        MYLOG("[WIFI]Band: %s", wifi_band_txt(status.band));
        MYLOG("[WIFI]Channel: %d", status.channel);
        MYLOG("[WIFI]Security: %s", wifi_security_txt(status.security));
        MYLOG("[WIFI]RSSI: %d", status.rssi);
    }
}

void wifi_disconnect(void)
{
    struct net_if *iface = net_if_get_default();

    int ret = net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0);
    if (ret)
    {
        MYLOG("WiFi Disconnection Request Failed\n");
    }
}

