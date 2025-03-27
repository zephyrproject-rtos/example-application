// wifi_sm/wifiStateConnecting.cpp
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>

#include "wifiStateImp.hpp"
#include "wifiContext.hpp"
#include "myLogger.h"


wifiStateConnecting::wifiStateConnecting(wifiStateConnected* next): connected(next)
{

}

void wifiStateConnecting::enter(wifiContext& ctx, net_if* _iface)
{
    MYLOG("🔗 Entered Connecting state");
    iface = _iface;
    isAssociated = false;

    /* Take the SSID and Password from Environment Variables. */
    const std::string CONFIG_WIFI_SSID(WIFI_SSID);
    const std::string CONFIG_WIFI_PASSWORD(WIFI_PASSWORD);

    struct wifi_connect_req_params params =
    {
        .ssid = (const uint8_t*) CONFIG_WIFI_SSID.c_str(),
        .ssid_length = CONFIG_WIFI_SSID.length(),
        .psk = (const uint8_t*) (CONFIG_WIFI_PASSWORD.c_str()),
        .psk_length = CONFIG_WIFI_PASSWORD.length(),
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));

    if (ret)
    {
        MYLOG("Failed to connect to WiFi network! [Error]:%d", ret);
    }
    else
    {
        MYLOG("🔗 Connecting to Wi-Fi [SSID]: %s", CONFIG_WIFI_SSID.c_str());
    }
}

void wifiStateConnecting::handle(wifiContext& ctx, wifi_iface_status status)
{
    // struct wifi_iface_status status = {0};

    // int ret = net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, iface, &status,
    //                         sizeof(struct wifi_iface_status));

    // if (ret)
    // {
    //     MYLOG("WiFi Status Request Failed");
    // }

    if (status.state >= WIFI_STATE_ASSOCIATED && !isAssociated)
    {
        MYLOG("WiFi is Associated");
        isAssociated = true;
    }

    if (status.state == WIFI_STATE_COMPLETED && isAssociated)
    {
        /* Check for IP address assignment */
        struct net_if_ipv4 *ipv4 = net_if_get_config(iface)->ip.ipv4;

        if (ipv4 && ipv4->unicast[0].ipv4.is_used)
        {
            struct in_addr addr = ipv4->unicast[0].ipv4.address.in_addr;
            char ip_str[NET_IPV4_ADDR_LEN];

            net_addr_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));
            MYLOG("✅ IP address assigned: %s", ip_str);

            MYLOG("✅ Connected. Switching to Connected state...");
            isConnectedCalled = false;
            ctx.setState(static_cast<wifiState*>(connected));
        }
        else
        {
            // MYLOG("IP address not assigned yet");
        }
    }
}

const char* wifiStateConnecting::name() const
{
    return "Connecting";
}

void wifiStateConnecting::setConnectedCalled(bool value)
{
    isConnectedCalled = value;
}

bool wifiStateConnecting::getConnectedCalled()
{
    return isConnectedCalled;
}