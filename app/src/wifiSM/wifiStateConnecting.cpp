// wifi_sm/wifiStateConnecting.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"

wifiStateConnecting::wifiStateConnecting(wifiStateConnected* next)
    : connected(next) {}

void wifiStateConnecting::enter(wifiContext& ctx) {
    MYLOG("🔗 Entered Connecting state");
}

void wifiStateConnecting::handle(wifiContext& ctx) {
    MYLOG("✅ Connected. Switching to Connected state...");
    ctx.setState(static_cast<wifiState*>(connected));
}

const char* wifiStateConnecting::name() const {
    return "Connecting";
}
