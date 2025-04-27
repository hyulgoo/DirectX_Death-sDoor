#pragma once
#include "CIconScript.h"
class CMPIconScript :
    public CIconScript
{
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void SetMP(int _iCurMP, PLAYER_MAGIC iCurMagic) override;

    CLONE(CMPIconScript);

public:
    CMPIconScript();
    ~CMPIconScript();
};

