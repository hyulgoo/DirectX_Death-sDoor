#pragma once
#include "CState.h"
class CKnightSpinAttackCombo :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightSpinAttackCombo);

public:
    CKnightSpinAttackCombo();
    ~CKnightSpinAttackCombo();
};

