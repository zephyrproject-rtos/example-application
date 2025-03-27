#include "wifiStateImp.hpp"
#include "myLogger.h"


void wifiStateError::enter(wifiContext& ctx)
{
    MYLOG("🛑 Entered Error state");
}

void wifiStateError::handle(wifiContext& ctx)
{
    MYLOG("⚠️ Handling error... staying here");
    // Optionally retry or escalate
}

const char* name()
{
    return "Error";
}

