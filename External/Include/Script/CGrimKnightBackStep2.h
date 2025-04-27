#pragma once
#include "CState.h"
class CGrimKnightBackStep2 :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGrimKnightBackStep2);

public:
    CGrimKnightBackStep2();
    ~CGrimKnightBackStep2();
};

