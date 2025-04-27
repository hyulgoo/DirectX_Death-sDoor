#pragma once
#include "CState.h"
class CGruntJumpAttackReady :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntJumpAttackReady);

public:
    CGruntJumpAttackReady();
    ~CGruntJumpAttackReady();
};

