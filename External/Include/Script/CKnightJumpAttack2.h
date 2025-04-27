#pragma once
#include "CState.h"
class CKnightJumpAttack2 :
    public CState
{
private:
    float   m_fDelay;
    bool    m_bAttack;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightJumpAttack2);

public:
    CKnightJumpAttack2();
    ~CKnightJumpAttack2();
};

