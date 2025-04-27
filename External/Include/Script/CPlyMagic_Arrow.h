#pragma once
#include "CState.h"
class CPlyMagic_Arrow :
    public CState
{
private:
    Vec3            m_vAttackDir;
    CGameObject*    m_pArrow;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    void CalcDir();

    CLONE(CPlyMagic_Arrow);

public:
    CPlyMagic_Arrow();
    ~CPlyMagic_Arrow();
};

