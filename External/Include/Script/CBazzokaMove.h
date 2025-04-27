#pragma once
#include "CState.h"
class CBazzokaMove :
    public CState
{
private:
    float   m_fLastRenewal;
    float   m_fRenewal_Trace; // 추적 경로를 갱신하는 주기
    Vec3    m_vActualPath[256];
    int     m_iActualPathCount;
    int     m_iCurrentPathIndex;
    float   m_fMoveTime;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    CLONE(CBazzokaMove);

public:
    CBazzokaMove();
    ~CBazzokaMove();
};