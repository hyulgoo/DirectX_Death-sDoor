#pragma once
#include "CState.h"
class CPlyDead :
    public CState
{
private:
    CGameObject*    m_pDeathTex;
    float           m_fTimetoStartPoint;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyDead);

public:
    CPlyDead();
    ~CPlyDead();
};

