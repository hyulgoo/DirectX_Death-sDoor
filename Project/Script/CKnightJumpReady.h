#pragma once
#include "CState.h"
class CKnightJumpReady :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightJumpReady);

public:
    CKnightJumpReady();
    ~CKnightJumpReady();
};

