#include "wifiContext.hpp"

wifiContext::wifiContext(wifiState* initial, net_if* _iface) : state(initial)
{
    iface = _iface;
    state->enter(*this, iface);
}

void wifiContext::setState(wifiState* newState)
{
    state = newState;
    MYLOG("🔁 Transitioned to state: %s", state->name());
    state->enter(*this, iface);
}

void wifiContext::update(wifi_iface_status status)
{
    if (state)
    {
        state->handle(*this, status);
    }
}