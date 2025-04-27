#pragma once
#include "CState.h"
class CCrowBossSpitting :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CCrowBossSpitting);

public:
    CCrowBossSpitting();
    ~CCrowBossSpitting();
};

