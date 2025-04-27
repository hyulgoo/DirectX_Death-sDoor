#pragma once
#include "CState.h"
class CGrimKnightSpinDown :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGrimKnightSpinDown);

public:
    CGrimKnightSpinDown();
    ~CGrimKnightSpinDown();
};

