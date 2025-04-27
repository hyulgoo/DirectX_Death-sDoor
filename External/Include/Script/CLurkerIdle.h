#pragma once
#include "CState.h"
class CLurkerIdle :
    public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    CLONE(CLurkerIdle);

public:
    CLurkerIdle();
    ~CLurkerIdle();
};

