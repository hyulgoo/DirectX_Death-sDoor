#pragma once
#include "CIconScript.h"
class CHookIconScript :
    public CIconScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void MagicActive(bool _bActive) override;

    CLONE(CHookIconScript);

public:
    CHookIconScript();
    ~CHookIconScript();
};

