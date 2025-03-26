// wifi_sm/wifiStateConnected.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"

wifiStateConnected::wifiStateConnected(wifiStateDisconnected* next)
    : disconnected(next) {}

void wifiStateConnected::enter(wifiContext& ctx) {
    MYLOG("📶 Connected! Holding...");
}

void wifiStateConnected::handle(wifiContext& ctx) {
    MYLOG("📡 Maintaining connection");
    bool lostConnection = true; // Replace with actual logic
    if (lostConnection) {
        ctx.setState(static_cast<wifiState*>(disconnected));
    }
}

const char* wifiStateConnected::name() const {
    return "Connected";
}
