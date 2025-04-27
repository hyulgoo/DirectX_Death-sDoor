#pragma once
#include "CState.h"
class CCrowBossWalk :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CCrowBossWalk);

public:
    CCrowBossWalk();
    ~CCrowBossWalk();
};

