#pragma once
#include "CState.h"
class CGrimKnightGuardBreak :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGrimKnightGuardBreak);

public:
    CGrimKnightGuardBreak();
    ~CGrimKnightGuardBreak();
};

