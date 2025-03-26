#include "wifiManager.hpp"
#include "managerFactory.hpp"
#include "myLogger.h"

/* Anonymous namespace limits the visibility of `registered` to this file only,
   preventing linker conflicts with other translation units.
*/
namespace
{
/*
    [] { ... }      This is an anonymous lambda function (no capture)
    ()              This immediately calls it — IIFE style (Immediately Invoked Function Expression)

    Does the following:
    1 . Defines a lambda
        - It registers "wifi" with a creation lambda [] { return new wifiManager(); }
        - Then returns true
    2. Immediately invokes the lambda with ()
    3. Stores the result (true) into a bool registered

    To ensure this line runs exactly once at startup (usually when wifiManager.cpp is loaded).
    It’s a clean trick to register the manager without requiring the user to manually call
    registerManager() somewhere in main().

    1. Automatic Registration
    It ensures the manager registers itself with the factory — without needing anyone to call
    registerManager() manually in main() or somewhere else.
    Think of it like:
        “Hey, if this manager is compiled in, I want it automatically available!”

    2. Only Runs Once
    Because it's a static variable, it’s initialized exactly once — before main() begins.

    3. Clean Code
    This keeps registration logic with the class definition itself — no need to edit a
    centralized file or factory list every time you add a new manager.
*/
bool registered = [] {
    managerFactory::registerManager("wifi", [] { return new wifiManager(); });
    return true;
}();

}

void wifiManager::init()
{
    MYLOG("[wifiManager] Initialization started");
    /* Initialization logic here */

    idle = new wifiStateIdle(nullptr);
    disconnected = new wifiStateDisconnected(idle);
    connected = new wifiStateConnected(disconnected);
    connecting = new wifiStateConnecting(connected);

    /* Connect the loop back */
    *idle = wifiStateIdle(connecting);

    /* StateMachine Context */
    context = new wifiContext(idle);
    MYLOG("🚀 [wifiManager] Starting Wi-Fi State Machine");
}

void wifiManager::tick()
{
    if (context)
    {
        context->update();
    }
}

const char* wifiManager::name() const
{
    return "wifiManager";
}

