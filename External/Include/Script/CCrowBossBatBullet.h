#pragma once
#include "CState.h"
class CCrowBossBatBullet :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CCrowBossBatBullet);

public:
    CCrowBossBatBullet();
    ~CCrowBossBatBullet();
};

