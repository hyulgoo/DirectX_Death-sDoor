#pragma once
#include "CState.h"
class CKnightJumpAttack :
    public CState
{
private:
    float m_fTime;
    bool m_bCameraShake;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightJumpAttack);

public:
    CKnightJumpAttack();
    ~CKnightJumpAttack();
};

