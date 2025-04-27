#pragma once
#include "CState.h"
class CGruntRun2 :
    public CState
{
private:
    float   m_fLastRenewal;
    float   m_fRenewal_Trace; // 추적 경로를 갱신하는 주기
    Vec3    m_vActualPath[256];
    int     m_iActualPathCount;
    int     m_iCurrentPathIndex;
    float   m_fCoolTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntRun2);

public:
    CGruntRun2();
    ~CGruntRun2();
};

