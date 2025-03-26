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
void wifiManager::init()
{
    MYLOG("[wifiManager] Initialization started");
    /* Initialization logic here */

    register_wifi_events();

    iface = net_if_get_default();

    if (iface)
    {
        net_if_up(iface);
        MYLOG("Network interface found!");
    }
    else
    {
        MYLOG("No network interface found!");
    }

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

    // scan();
    // MYLOG("Waiting for Scan to complete");
    // while(isScanComplete == false)
    // {
    //     // k_sleep(K_MSEC(1000));
    // }
    // wifi_status();

    /* State Machine Initialization */
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
    struct wifi_iface_status status = {0};

    status = get_wifi_status(iface);

    if (isConnecting)
    {
        if (status.state >= WIFI_STATE_ASSOCIATED)
        {
            struct net_if_ipv4 *ipv4 = net_if_get_config(iface)->ip.ipv4;

            if (ipv4 && ipv4->unicast[0].ipv4.is_used)
            {
                struct in_addr addr = ipv4->unicast[0].ipv4.address.in_addr;
                char ip_str[NET_IPV4_ADDR_LEN];

                net_addr_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));
                MYLOG("✅ IP address assigned: %s", ip_str);

                isIpObtained = true;
            }

            if (isIpObtained)
            {
                isConnecting = false;
                connecting->setConnectedCalled(true);
                context->setState(connected);
            }
        }
    }

    if (context)
    {
        context->update();
    }
}

void wifiManager::connect()
{
    MYLOG("🔗 Connecting to Wi-Fi");

    struct wifi_iface_status status = get_wifi_status(iface);
    wifi_status();

    if (status.state == WIFI_STATE_INACTIVE || status.state == WIFI_STATE_DISCONNECTED)
    {
        MYLOG("WiFi is Inactive or Disconnected");
        /* Take the SSID and Password from Environment Variables. */
        const std::string CONFIG_WIFI_SSID(WIFI_SSID);
        const std::string CONFIG_WIFI_PASSWORD(WIFI_PASSWORD);

        struct wifi_connect_req_params params =
        {
            .ssid = (const uint8_t*) (CONFIG_WIFI_SSID.c_str()),
            .ssid_length = CONFIG_WIFI_SSID.length(),
            .psk = (const uint8_t*) (CONFIG_WIFI_PASSWORD.c_str()),
            .psk_length = CONFIG_WIFI_PASSWORD.length(),
            .security = WIFI_SECURITY_TYPE_PSK,
        };

        MYLOG("Connecting to the Wifi Network [SSID]: %s", CONFIG_WIFI_SSID.c_str());

        int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
        if (ret)
        {
            MYLOG("Failed to connect to WiFi network! [Error]:%d", ret);
        }
        else
        {
            isConnecting = true;
            idle->setConnectingCalled(true);
        }
    }
    else
    {
        MYLOG("WiFi is already connected");
    }
}

void wifiManager::disconnect()
{
    MYLOG("❌ Disconnecting from Wi-Fi");

    int ret = net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0);
    if (ret)
    {
        MYLOG("WiFi Disconnection Request Failed\n");
    }
    else
    {
        connected->setDisconnectCalled(true);
    }
}

void wifiManager::scan()
{
    MYLOG("🔍 Scanning for Wi-Fi Networks");

    struct wifi_scan_params params;

    int ret = net_mgmt(NET_REQUEST_WIFI_SCAN, iface, &params, sizeof(params));
    if (ret)
    {
        MYLOG("WiFi Scan Request Failed\n");
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

void wifiManager::wifi_status()
{
    struct wifi_iface_status status = {0};

    status = get_wifi_status(iface);

    MYLOG("Wifi Status: %d\n", status.state);

    if (status.state >= WIFI_STATE_ASSOCIATED)
    {
        MYLOG("[WIFI]SSID: %-32s", status.ssid);
        MYLOG("[WIFI]Band: %s", wifi_band_txt(status.band));
        MYLOG("[WIFI]Channel: %d", status.channel);
        MYLOG("[WIFI]Security: %s", wifi_security_txt(status.security));
        MYLOG("[WIFI]RSSI: %d", status.rssi);
    }
}

void wifiManager::setScanComplete(bool value)
{
    isScanComplete = value;
}

const char* wifiManager::name() const
{
    return "wifiManager";
}
