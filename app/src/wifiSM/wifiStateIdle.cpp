// wifi_sm/wifiStateIdle.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"
#include "myLogger.h"
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>

wifiStateIdle::wifiStateIdle(wifiStateConnecting* next) : connecting(next)
{

}

void wifiStateIdle::enter(wifiContext& ctx, net_if* _iface)
{
    MYLOG("🟡 Entered Idle state");
    isConnectingCalled = false;
    iface = _iface;
}

void wifiStateIdle::handle(wifiContext& ctx, wifi_iface_status status)
{
    if (isConnectingCalled)
    {
        MYLOG("🟡 Handling Idle state... transitioning to Connecting");

        if (status.state == WIFI_STATE_INACTIVE || status.state == WIFI_STATE_DISCONNECTED)
        {
            MYLOG("WiFi is Inactive or Disconnected");
        //     /* Take the SSID and Password from Environment Variables. */
        //     const std::string CONFIG_WIFI_SSID(WIFI_SSID);
        //     const std::string CONFIG_WIFI_PASSWORD(WIFI_PASSWORD);

        //     struct wifi_connect_req_params params =
        //     {
        //         .ssid = (const uint8_t*) CONFIG_WIFI_SSID.c_str(),
        //         .ssid_length = CONFIG_WIFI_SSID.length(),
        //         .psk = (const uint8_t*) (CONFIG_WIFI_PASSWORD.c_str()),
        //         .psk_length = CONFIG_WIFI_PASSWORD.length(),
        //         .security = WIFI_SECURITY_TYPE_PSK,
        //     };

        //     MYLOG("Connecting to the Wifi Network [SSID]: %s", CONFIG_WIFI_SSID.c_str());

        //     int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));

        //     if (ret)
        //     {
        //         MYLOG("Failed to connect to WiFi network! [Error]:%d", ret);
        //     }
        //     else
        //     {
                MYLOG("➡️ Transitioning to Connecting...");
                ctx.setState(connecting);
        //     }
        }
    }
}

const char* wifiStateIdle::name() const
{
    return "Idle";
}

void wifiStateIdle::setConnectingCalled(bool value)
{
    isConnectingCalled = value;
}

bool wifiStateIdle::getConnectingCalled()
{
    return isConnectingCalled;
}