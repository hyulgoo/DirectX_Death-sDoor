#pragma once
#include "CState.h"
class CGruntWalk :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntWalk);

public:
    CGruntWalk();
    ~CGruntWalk();
};

