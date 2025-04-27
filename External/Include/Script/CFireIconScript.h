#pragma once
#include "CIconScript.h"
class CFireIconScript :
    public CIconScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void MagicActive(bool _bActive) override;

    CLONE(CFireIconScript);

public:
    CFireIconScript();
    ~CFireIconScript();
};

