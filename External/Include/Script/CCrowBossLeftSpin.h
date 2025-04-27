#pragma once
#include "CState.h"
class CCrowBossLeftSpin :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CCrowBossLeftSpin);

public:
    CCrowBossLeftSpin();
    ~CCrowBossLeftSpin();
};

