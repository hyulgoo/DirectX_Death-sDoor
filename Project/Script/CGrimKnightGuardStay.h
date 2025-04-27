#pragma once
#include "CState.h"
class CGrimKnightGuardStay :
    public CState
{
private:
    int     m_iHitCount;
    int     m_iPrevHP;
    int     m_iOriginMaxHP;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGrimKnightGuardStay);

public:
    CGrimKnightGuardStay();
    ~CGrimKnightGuardStay();
};

