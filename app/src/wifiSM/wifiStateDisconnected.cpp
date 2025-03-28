// wifi_sm/wifiStateDisconnected.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"

wifiStateDisconnected::wifiStateDisconnected(wifiStateIdle* next): idle(next)
{

}

void wifiStateDisconnected::enter(wifiContext& ctx, net_if* iface)
{
    MYLOG("❌ Disconnected. Awaiting reconnection...");
}

void wifiStateDisconnected::handle(wifiContext& ctx, wifi_iface_status status)
{
    MYLOG("🔄 Trying to recover or restart connection");
    bool recoveryComplete = true; // Replace with real logic
    if (recoveryComplete)
    {
        ctx.setState(static_cast<wifiState*>(idle));
    }
    // if (isConnectCalled)
    // {
    //     MYLOG("🔗 Connecting. Switching to Connecting state...");
    //     ctx.setState(static_cast<wifiState*>(connecting));
    //     isConnectCalled = false;
    // }
}

int wifiStateDisconnected::name() const
{
    return static_cast<int>(CONNECTED);
}

void wifiStateDisconnected::setConnectCalled(bool value)
{
    isConnectCalled = value;
}

bool wifiStateDisconnected::getConnectCalled()
{
    return isConnectCalled;
}