#pragma once
#include "CState.h"
class CGruntLeftMove :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntLeftMove);

public:
    CGruntLeftMove();
    ~CGruntLeftMove();
};

