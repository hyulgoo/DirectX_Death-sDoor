#pragma once
#include "CState.h"
class CKnightChopAttackComboEnd :
    public CState
{
private:
    float m_fTime;
    bool  m_bCameraShake;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightChopAttackComboEnd);

public:
    CKnightChopAttackComboEnd();
    ~CKnightChopAttackComboEnd();
};

