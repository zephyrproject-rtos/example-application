// wifiSM/wifiState.hpp
#pragma once

class wifiContext;  // Forward declaration

class wifiState {
public:
    virtual void enter(wifiContext& ctx) = 0;
    virtual void handle(wifiContext& ctx) = 0;
    virtual const char* name() const = 0;
    virtual ~wifiState() = default;
};