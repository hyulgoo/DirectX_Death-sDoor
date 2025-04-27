#pragma once
#include "CState.h"
class CKnightChopAttackEnd :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightChopAttackEnd);

public:
    CKnightChopAttackEnd();
    ~CKnightChopAttackEnd();
};

