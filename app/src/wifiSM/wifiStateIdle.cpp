// wifi_sm/wifiStateIdle.cpp
#include "wifiStateImp.hpp"
#include "wifiContext.hpp"

wifiStateIdle::wifiStateIdle(wifiStateConnecting* next)
    : connecting(next) {}

void wifiStateIdle::enter(wifiContext& ctx) {
    MYLOG("🟡 Entered Idle state");
}

void wifiStateIdle::handle(wifiContext& ctx) {
    MYLOG("➡️ Transitioning to Connecting...");
    ctx.setState(connecting);
}

const char* wifiStateIdle::name() const {
    return "Idle";
}
