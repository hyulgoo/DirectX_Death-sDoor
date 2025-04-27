#pragma once
#include "CState.h"
class CPlyWpMagic_Fire :
    public CState
{
private:
    float m_fChargeDelay;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpMagic_Fire);

public:
    CPlyWpMagic_Fire();
    ~CPlyWpMagic_Fire();
};

