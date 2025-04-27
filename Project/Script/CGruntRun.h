#pragma once
#include "CState.h"
class CGruntRun :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntRun);

public:
    CGruntRun();
    ~CGruntRun();
};

