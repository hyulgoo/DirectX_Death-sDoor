#pragma once
#include "CState.h"
class CCrowBossRun :
    public CState
{
private:
    int     m_iTime;
    Vec3   m_Dir;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CCrowBossRun);

public:
    CCrowBossRun();
    ~CCrowBossRun();
};

