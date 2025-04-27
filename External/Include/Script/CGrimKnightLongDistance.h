#pragma once
#include "CState.h"
class CGrimKnightLongDistance :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGrimKnightLongDistance);

public:
    CGrimKnightLongDistance();
    ~CGrimKnightLongDistance();
};

