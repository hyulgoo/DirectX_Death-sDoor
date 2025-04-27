#pragma once
#include "CState.h"
class CPlyWpRun :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpRun);

public:
    CPlyWpRun();
    ~CPlyWpRun();
};

