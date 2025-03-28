#pragma once
#include "wifiState.hpp"
#include "myLogger.h"

#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

typedef enum
{
    IDLE = 0,
    CONNECTING,
    CONNECTED,
    DISCONNECTED,
    ERROR,
} wifiStateEnum;

class wifiContext
{
private:
    wifiState* state;
    net_if* iface;

public:
    wifiContext(wifiState* initial, net_if* iface);
    void setState(wifiState* newState);
    wifiStateEnum  getState();
    const char *getStateName();
    void update(wifi_iface_status status);
};
