// wifiSM/wifiState.hpp
#pragma once

#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

class wifiContext;  // Forward declaration

class wifiState
{
public:
    virtual void enter(wifiContext& ctx, net_if* iface) = 0;
    virtual void handle(wifiContext& ctx, wifi_iface_status status) = 0;
    virtual int name() const = 0;
    virtual ~wifiState() = default;
};