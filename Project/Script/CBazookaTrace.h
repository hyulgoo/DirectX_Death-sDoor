#pragma once
#include "CState.h"
class CBazookaTrace :
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

    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);
    virtual void EndOverlap(CCollider3D* _Other);

    CLONE(CBazookaTrace);

public:
    CBazookaTrace();
    ~CBazookaTrace();
};


