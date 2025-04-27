#pragma once
#include "CState.h"
class CPlyWpWalk :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpWalk);

public:
    CPlyWpWalk();
    ~CPlyWpWalk();
};

