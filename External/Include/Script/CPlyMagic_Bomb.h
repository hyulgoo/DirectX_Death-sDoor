#pragma once
#include "CState.h"
class CPlyMagic_Bomb :
    public CState
{
    Vec3            m_vAttackDir;
    CGameObject*    m_pBomb;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    void CalcDir();

    CLONE(CPlyMagic_Bomb);

public:
    CPlyMagic_Bomb();
    ~CPlyMagic_Bomb();
};

