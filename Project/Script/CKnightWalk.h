#pragma once
#include "CState.h"
class CKnightWalk :
    public CState
{
private:
    float m_fWalkSoundDelay;
    bool m_bAttackOrder;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightWalk);

public:
    CKnightWalk();
    ~CKnightWalk();
};

