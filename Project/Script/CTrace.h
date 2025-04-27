#pragma once
#include "CState.h"
class CTrace :
    public CState
{
private:
    float m_fLastRenewal;
    float m_fRenewal_Trace; // 추적 경로를 갱신하는 주기
    Vec3  m_vActualPath[256];
    int   m_iActualPathCount;
    int   m_iCurrentPathIndex;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CTrace);

public:
    CTrace();
    ~CTrace();
};

