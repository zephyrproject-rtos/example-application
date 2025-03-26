#pragma once
#include "wifiState.hpp"
#include "myLogger.h"

class wifiContext {
private:
    wifiState* state;

public:
    wifiContext(wifiState* initial);
    void setState(wifiState* newState);
    void update();
};
