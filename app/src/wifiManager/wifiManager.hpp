// wifiManager.hpp
#pragma once
#include "iManager.hpp"

/* State Machine inlcudes */
#include "wifiContext.hpp"
#include "wifiStateImp.hpp"
#include "wifiManager.hpp"
class wifiManager : public iManager
{
private:
    wifiContext* context;
    wifiStateIdle* idle;
    wifiStateDisconnected* disconnected;
    wifiStateConnected* connected;
    wifiStateConnecting* connecting;
public:
    void init() override;
    void tick() override;
    const char* name() const override;
};
