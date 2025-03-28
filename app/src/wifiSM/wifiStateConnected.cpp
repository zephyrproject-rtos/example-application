// wifi_sm/wifiStateConnected.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"

#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>

wifiStateConnected::wifiStateConnected(wifiStateDisconnected* next): disconnected(next)
{

}

void wifiStateConnected::enter(wifiContext& ctx, net_if* iface)
{
    MYLOG("📶 Connected! Holding...");
    MYLOG("📡 Maintaining connection");
}

void wifiStateConnected::handle(wifiContext& ctx, wifi_iface_status status)
{
    // MYLOG("📡 Maintaining connection");
    bool lostConnection = false; // Replace with actual logic
    if (lostConnection)
    {
        MYLOG("❌ Connection lost. Switching to Disconnected state...");
        ctx.setState(static_cast<wifiState*>(disconnected));
    }
    if (isDisconnectCalled)
    {
        MYLOG("❌ Disconnecting. Switching to Disconnected state...");

        ctx.setState(static_cast<wifiState*>(disconnected));

        struct net_if *iface = net_if_get_default();

        int ret = net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0);
        if (ret)
        {
            MYLOG("WiFi Disconnection Request Failed\n");
        }
        isDisconnectCalled = false;
    }
}

int wifiStateConnected::name() const
{
    return static_cast<int>(CONNECTED);
}

void wifiStateConnected::setDisconnectCalled(bool value)
{
    isDisconnectCalled = value;
}

bool wifiStateConnected::getDisconnectCalled()
{
    return isDisconnectCalled;
}