#pragma once
#include "CState.h"
class CCrowBossGuidedBullet :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CCrowBossGuidedBullet);

public:
    CCrowBossGuidedBullet();
    ~CCrowBossGuidedBullet();
};

