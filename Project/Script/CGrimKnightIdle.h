#pragma once
#include "CState.h"
class CGrimKnightIdle :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGrimKnightIdle);

public:
    CGrimKnightIdle();
    ~CGrimKnightIdle();
};

