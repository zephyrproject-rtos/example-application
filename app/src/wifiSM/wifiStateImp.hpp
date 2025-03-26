#pragma once
#include "wifiState.hpp"
#include "wifiContext.hpp"

class wifiStateIdle; // Forward declaration

// State: Disconnected
class wifiStateDisconnected : public wifiState {
private:
    wifiStateIdle* idle;

public:
    wifiStateDisconnected(wifiStateIdle* next);
    void enter(wifiContext& ctx) override;
    void handle(wifiContext& ctx) override;
    const char* name() const override;
};

// State: Connected
class wifiStateConnected : public wifiState {
private:
    bool isDisconnectCalled = false;
    wifiStateDisconnected* disconnected;

public:
    wifiStateConnected(wifiStateDisconnected* next);
    void enter(wifiContext& ctx) override;
    void handle(wifiContext& ctx) override;
    const char* name() const override;
    void setDisconnectCalled(bool value);
    bool getDisconnectCalled();
};

// State: Connecting
class wifiStateConnecting : public wifiState {
private:
    bool isConnectedCalled = false;
    wifiStateConnected* connected;

public:
    wifiStateConnecting(wifiStateConnected* next);
    void enter(wifiContext& ctx) override;
    void handle(wifiContext& ctx) override;
    const char* name() const override;
    void setConnectedCalled(bool value);
    bool getConnectedCalled();
};

// State: Idle
class wifiStateIdle : public wifiState {
private:
    bool isConnectingCalled = false;
    wifiStateConnecting* connecting;

public:
    wifiStateIdle(wifiStateConnecting* next);
    void enter(wifiContext& ctx) override;
    void handle(wifiContext& ctx) override;
    const char* name() const override;
    void setConnectingCalled(bool value);
    bool getConnectingCalled();
};
