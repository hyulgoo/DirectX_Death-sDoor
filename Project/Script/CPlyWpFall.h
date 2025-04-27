#pragma once
#include "CState.h"
class CPlyWpFall :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpFall);

public:
    CPlyWpFall();
    ~CPlyWpFall();
};

