#pragma once
#include "CState.h"
class CGruntTrace :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntTrace);

public:
    CGruntTrace();
    ~CGruntTrace();
};

