// wifi_sm/wifiStateConnecting.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"

wifiStateConnecting::wifiStateConnecting(wifiStateConnected* next)
    : connected(next) {}

void wifiStateConnecting::enter(wifiContext& ctx)
{
    MYLOG("🔗 Entered Connecting state");
}

void wifiStateConnecting::handle(wifiContext& ctx)
{
    if (isConnectedCalled)
    {
        MYLOG("✅ Connected. Switching to Connected state...");
        ctx.setState(static_cast<wifiState*>(connected));
        isConnectedCalled = false;
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