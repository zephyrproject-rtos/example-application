#pragma once
#include "wifiState.hpp"
#include "myLogger.h"

#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
class wifiContext {
private:
    wifiState* state;
    net_if* iface;

public:
    wifiContext(wifiState* initial, net_if* iface);
    void setState(wifiState* newState);
    void update(wifi_iface_status status);
};
