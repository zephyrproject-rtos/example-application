#include "wifiContext.hpp"

wifiContext::wifiContext(wifiState* initial) : state(initial)
{
    state->enter(*this);
}

void wifiContext::setState(wifiState* newState)
{
    state = newState;
    MYLOG("🔁 Transitioned to state: %s", state->name());
    state->enter(*this);
}

void wifiContext::update()
{
    if (state)
    {
        state->handle(*this);
    }
}