#pragma once
#include "CState.h"
class CLurkerFrontMove :
    public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    CLONE(CLurkerFrontMove);

public:
    CLurkerFrontMove();
    ~CLurkerFrontMove();
};

