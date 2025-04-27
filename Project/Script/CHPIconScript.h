#pragma once
#include "CIconScript.h"
class CHPIconScript :
    public CIconScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void SetHP(int _iCurHP) override;

    CLONE(CHPIconScript);

public:
    CHPIconScript();
    ~CHPIconScript();
};

