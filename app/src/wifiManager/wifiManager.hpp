// wifiManager.hpp
#pragma once
#include "iManager.hpp"

class wifiManager : public iManager
{
public:
    void init() override;
    const char* name() const override;
};
