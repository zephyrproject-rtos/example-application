#include "wifiManager.hpp"
#include "managerFactory.hpp"
#include "myLogger.h"

#include <zephyr/net/wifi_mgmt.h>
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

static struct net_mgmt_event_callback wifi_cb;
static struct net_mgmt_event_callback ipv4_cb;

wifiManager* wifiManager::getInstance()
{
    return this;
}

void wifiManager::register_wifi_events()
{
    net_mgmt_init_event_callback(&wifi_cb, this->wifi_mgmt_event_handler,
                                    NET_EVENT_WIFI_CONNECT_RESULT |
                                    NET_EVENT_WIFI_DISCONNECT_RESULT |
                                    NET_EVENT_WIFI_SCAN_RESULT );

    net_mgmt_init_event_callback(&ipv4_cb, this->ipv4_mgmt_event_handler, NET_EVENT_IPV4_ADDR_ADD);

    net_mgmt_add_event_callback(&wifi_cb);
    net_mgmt_add_event_callback(&ipv4_cb);

}

void wifiManager::reinit()
{
    struct wifi_iface_status status = get_wifi_status(iface);

    while (status.state != WIFI_STATE_COMPLETED)
    {
        if ((status.state == WIFI_STATE_INACTIVE) || (status.state == WIFI_STATE_DISCONNECTED))
        {
            break;
        }
        /* Wait for Wifi to be in known state */
        status = get_wifi_status(iface);
    }

    // struct wifi_connect_req_params params =
    // {
    //     .ssid = (const uint8_t*) (WIFI_SSID),
    //     .ssid_length = strlen(WIFI_SSID),
    //     .psk = (const uint8_t*) (WIFI_PASSWORD),
    //     .psk_length = strlen(WIFI_PASSWORD),
    //     .security = WIFI_SECURITY_TYPE_PSK,
    // };

    // MYLOG("Connecting to the Wifi Network [SSID]: %s", WIFI_SSID);

    // int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
    // if (ret)
    // {
    //     MYLOG("Failed to connect to WiFi network! [Error]:%d", ret);
    // }

    MYLOG("🚀 [wifiManager] Starting Wi-Fi State Machine");

}

void wifiManager::init()
{
    MYLOG("[wifiManager] Initialization started");

    /* Initialization logic here */

    /* State Machine Initialization */
    idle = new wifiStateIdle(nullptr);
    disconnected = new wifiStateDisconnected(idle);
    connected = new wifiStateConnected(disconnected);
    connecting = new wifiStateConnecting(connected);
    error = new wifiStateError(idle);

    /* Connect the loop back */
    *idle = wifiStateIdle(connecting);

    /* Get the WiFi Interface */
    // for (struct net_if* it = net_if_get_first(); it != NULL; it = net_if_get_next(it))
    // {
    //     if (net_if_l2(it) == &NET_L2_GET_NAME(WIFI_MGMT))
    //     {
    //         iface = it;
    //         break;
    //     }
    // }

    iface  = net_if_get_first_wifi();

    /* StateMachine Context */
    context = new wifiContext(idle, iface);

    /* Register the WiFi Event Handlers */
    register_wifi_events();

    if (iface)
    {
        MYLOG("Network interface found!");
        isError = false;
    }
    else
    {
        MYLOG("No network interface found!");
        isError = true;
    }

    if (net_if_is_up(iface))
    {
        MYLOG("Network interface is up");
    }
    else
    {
        MYLOG("Network interface is down");
        if (net_if_up(iface) != 0)
        {
            MYLOG("Couldnt bring Network Interface to up state");
        }
        else
        {
            MYLOG("Network interface is up");
        }
    }

    if (!isError)
    {
        reinit();
    }
    else
    {
        state = ERROR;
        context->setState(static_cast<wifiState*>(error));
    }

}

void wifiManager::tick()
{
    struct wifi_iface_status status = {0};

    status = get_wifi_status(iface);

    if (context)
    {
        context->update(status);
        state = context->getState();
    }
}

void wifiManager::connect()
{
    if (!isError)
    {
        MYLOG("🔗 Connecting to Wi-Fi");
        idle->setConnectingCalled(true);
    }
    else
    {
        MYLOG("❌ Error in Wi-Fi Initialization. Cannot Connect");
        context->setState(static_cast<wifiState*>(error));
    }
}

void wifiManager::disconnect()
{
    MYLOG("❌ Disconnecting from Wi-Fi");

    int ret = net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0);
    if (ret)
    {
        MYLOG("WiFi Disconnection Request Failed");
        state = ERROR;
        context->setState(static_cast<wifiState*>(error));
    }
    else
    {
        connected->setDisconnectCalled(true);
        state = DISCONNECTED;
    }
}

void wifiManager::scan()
{
    MYLOG("🔍 Scanning for Wi-Fi Networks");

    struct wifi_scan_params params;

    int ret = net_mgmt(NET_REQUEST_WIFI_SCAN, iface, &params, sizeof(params));
    if (ret)
    {
        MYLOG("WiFi Scan Request Failed");
    }
}

wifi_iface_status wifiManager::get_wifi_status(struct net_if* iface)
{
    struct wifi_iface_status status = {0};

    int ret = net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, iface, &status, sizeof(struct wifi_iface_status));
    if (ret)
    {
        MYLOG("WiFi Status Request Failed");
        return status;
    }

    return status;
}

wifiStateEnum wifiManager::getWifiState()
{
    return state;
}

wifiStateEnum wifiManager::wifi_status()
{
    struct wifi_iface_status status = {0};

    status = get_wifi_status(iface);

    MYLOG("Wifi Interface Status: %s", wifi_state_txt(static_cast<wifi_iface_state>(status.state)));

    if (status.state >= WIFI_STATE_ASSOCIATED)
    {
        MYLOG("[WIFI]SSID: %-32s", status.ssid);
        MYLOG("[WIFI]Band: %s", wifi_band_txt(status.band));
        MYLOG("[WIFI]Channel: %d", status.channel);
        MYLOG("[WIFI]Security: %s", wifi_security_txt(status.security));
        MYLOG("[WIFI]RSSI: %d", status.rssi);
    }

    return state;
}

void wifiManager::setScanComplete(bool value)
{
    isScanComplete = value;
}

const char* wifiManager::name() const
{
    return "wifiManager";
}
