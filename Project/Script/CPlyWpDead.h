#pragma once
#include "CState.h"
class CPlyWpDead :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpDead);

public:
    CPlyWpDead();
    ~CPlyWpDead();
};

