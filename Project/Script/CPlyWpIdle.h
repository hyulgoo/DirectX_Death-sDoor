#pragma once
#include "CState.h"
class CPlyWpIdle :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpIdle);

public:
    CPlyWpIdle();
    ~CPlyWpIdle();
};

