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
            MYLOG("➡️ Transitioning to Connecting...");
            ctx.setState(connecting);
        }
    }
}

int wifiStateIdle::name() const
{
    return static_cast<int>(IDLE);
}

void wifiStateIdle::setConnectingCalled(bool value)
{
    isConnectingCalled = value;
}

bool wifiStateIdle::getConnectingCalled()
{
    return isConnectingCalled;
}