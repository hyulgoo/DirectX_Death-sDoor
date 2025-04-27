#pragma once
#include "CState.h"
class CKnightRunAttack :
    public CState
{
private:
    float   m_fDelay;
    bool    m_bAttack;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightRunAttack);

public:
    CKnightRunAttack();
    ~CKnightRunAttack();
};

