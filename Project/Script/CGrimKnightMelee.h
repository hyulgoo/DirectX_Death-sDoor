#pragma once
#include "CState.h"
class CGrimKnightMelee :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGrimKnightMelee);

public:
    CGrimKnightMelee();
    ~CGrimKnightMelee();
};

