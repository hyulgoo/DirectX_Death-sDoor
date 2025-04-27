#pragma once
#include "CState.h"
class CGruntBackStep :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntBackStep);

public:
    CGruntBackStep();
    ~CGruntBackStep();
};

