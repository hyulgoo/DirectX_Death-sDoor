#pragma once
#include "CState.h"
class CGrimKnightSpinUp :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGrimKnightSpinUp);

public:
    CGrimKnightSpinUp();
    ~CGrimKnightSpinUp();
};

