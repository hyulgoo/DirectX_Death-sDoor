#pragma once
#include "CState.h"
class CPlyMagic_Fire :
    public CState
{
    Vec3            m_vAttackDir;
    CGameObject*    m_pFire;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    void CalcDir();

    CLONE(CPlyMagic_Fire);

public:
    CPlyMagic_Fire();
    ~CPlyMagic_Fire();
};

