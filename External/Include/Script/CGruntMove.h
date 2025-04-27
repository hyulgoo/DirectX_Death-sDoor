#pragma once
#include "CState.h"
class CGruntMove :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntMove);

public:
    CGruntMove();
    ~CGruntMove();
};

