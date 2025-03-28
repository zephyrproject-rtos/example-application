#pragma once
#include "wifiState.hpp"
#include "wifiContext.hpp"

#include <zephyr/net/net_if.h>

class wifiStateIdle; // Forward declaration

// State: Disconnected
class wifiStateDisconnected : public wifiState
{
private:
    bool isConnectCalled = false;
    wifiStateIdle* idle;

public:
    wifiStateDisconnected(wifiStateIdle* next);
    void enter(wifiContext& ctx, net_if* iface) override;
    void handle(wifiContext& ctx, wifi_iface_status status) override;
    int name(void) const override;
    void setConnectCalled(bool value);
    bool getConnectCalled(void);
};

// State: Connected
class wifiStateConnected : public wifiState
{
private:
    bool isDisconnectCalled = false;
    wifiStateDisconnected* disconnected;

public:
    wifiStateConnected(wifiStateDisconnected* next);
    void enter(wifiContext& ctx, net_if* iface) override;
    void handle(wifiContext& ctx, wifi_iface_status status) override;
    int name(void) const override;
    void setDisconnectCalled(bool value);
    bool getDisconnectCalled(void);
};

// State: Connecting
class wifiStateConnecting : public wifiState
{
private:
    bool isConnectedCalled = false;
    bool isAssociated = false;
    struct net_if* iface;
    wifiStateConnected* connected;
    wifiStateConnected* error;

public:
    wifiStateConnecting(wifiStateConnected* next);
    void enter(wifiContext& ctx, net_if* iface) override;
    void handle(wifiContext& ctx, wifi_iface_status status) override;
    int name(void) const override;
    void setConnectedCalled(bool value);
    bool getConnectedCalled(void);
};

// State: Idle
class wifiStateIdle : public wifiState
{
private:
    bool isConnectingCalled = false;
    struct net_if* iface;
    wifiStateConnecting* connecting;

public:
    wifiStateIdle(wifiStateConnecting* next);
    void enter(wifiContext& ctx, net_if* iface) override;
    void handle(wifiContext& ctx, wifi_iface_status status) override;
    int name(void) const override;
    void setConnectingCalled(bool value);
    bool getConnectingCalled(void);
};

// State: Error
class wifiStateError : public wifiState
{
private:
    wifiStateIdle* idle;

public:
    wifiStateError(wifiStateIdle* idleState);
    void enter(wifiContext& ctx, net_if* iface) override;
    void handle(wifiContext& ctx, wifi_iface_status status) override;
    int name(void) const override;
};
