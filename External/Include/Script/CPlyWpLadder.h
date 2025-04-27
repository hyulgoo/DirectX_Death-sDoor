#pragma once
#include "CState.h"
class CPlyWpLadder :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpLadder);

public:
    CPlyWpLadder();
    ~CPlyWpLadder();
};

