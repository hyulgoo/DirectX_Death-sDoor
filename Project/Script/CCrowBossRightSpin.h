#pragma once
#include "CState.h"
class CCrowBossRightSpin :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CCrowBossRightSpin);

public:
    CCrowBossRightSpin();
    ~CCrowBossRightSpin();
};

