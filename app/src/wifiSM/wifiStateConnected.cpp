// wifi_sm/wifiStateConnected.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"

wifiStateConnected::wifiStateConnected(wifiStateDisconnected* next)
    : disconnected(next) {}

void wifiStateConnected::enter(wifiContext& ctx)
{
    MYLOG("📶 Connected! Holding...");
}

void wifiStateConnected::handle(wifiContext& ctx)
{
    MYLOG("📡 Maintaining connection");
    bool lostConnection = true; // Replace with actual logic
    if (lostConnection)
    {
        MYLOG("❌ Connection lost. Switching to Disconnected state...");
        ctx.setState(static_cast<wifiState*>(disconnected));
    }
    if (isDisconnectCalled)
    {
        MYLOG("❌ Disconnecting. Switching to Disconnected state...");
        ctx.setState(static_cast<wifiState*>(disconnected));
        isDisconnectCalled = false;
    }
}

const char* wifiStateConnected::name() const
{
    return "Connected";
}

void wifiStateConnected::setDisconnectCalled(bool value)
{
    isDisconnectCalled = value;
}

bool wifiStateConnected::getDisconnectCalled()
{
    return isDisconnectCalled;
}