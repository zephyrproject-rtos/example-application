#pragma once

class iManager
{
public:
    virtual void init() = 0;
    virtual const char* name() const = 0;
    virtual ~iManager() = default;
};
