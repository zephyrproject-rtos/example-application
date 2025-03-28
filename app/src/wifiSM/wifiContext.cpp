#include "wifiContext.hpp"

wifiContext::wifiContext(wifiState* initial, net_if* _iface) : state(initial)
{
    iface = _iface;
    state->enter(*this, iface);
}

void wifiContext::setState(wifiState* newState)
{
    state = newState;
    MYLOG("🔁 Transitioned to state: %s", getStateName());
    state->enter(*this, iface);
}

void wifiContext::update(wifi_iface_status status)
{
    if (state)
    {
        state->handle(*this, status);
    }
}

const char * wifiContext::getStateName()
{
    switch(state->name())
    {
        case IDLE:
            return "IDLE";
        case CONNECTING:
            return "CONNECTING";
        case CONNECTED:
            return "CONNECTED";
        case DISCONNECTED:
            return "DISCONNECTED";
        case ERROR:
            return "ERROR";
        default:
            return "ERROR";
    }
}

wifiStateEnum wifiContext::getState()
{
    return static_cast<wifiStateEnum>(state->name());
}