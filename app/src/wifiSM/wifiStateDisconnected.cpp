// wifi_sm/wifiStateDisconnected.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"

wifiStateDisconnected::wifiStateDisconnected(wifiStateIdle* next): idle(next) {}

void wifiStateDisconnected::enter(wifiContext& ctx) {
    MYLOG("❌ Disconnected. Awaiting reconnection...");
}

void wifiStateDisconnected::handle(wifiContext& ctx) {
    MYLOG("🔄 Trying to recover or restart connection");
    bool recoveryComplete = true; // Replace with real logic
    if (recoveryComplete) {
        ctx.setState(static_cast<wifiState*>(idle));
    }
}

const char* wifiStateDisconnected::name() const {
    return "Disconnected";
}
