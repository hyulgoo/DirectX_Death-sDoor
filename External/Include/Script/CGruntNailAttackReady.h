#pragma once
#include "CState.h"
class CGruntNailAttackReady :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntNailAttackReady);

public:
    CGruntNailAttackReady();
    ~CGruntNailAttackReady();
};

