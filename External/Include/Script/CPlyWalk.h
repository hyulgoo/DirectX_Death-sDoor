#pragma once
#include "CState.h"
class CPlyWalk :
    public CState
{
private:
    float   m_fSpeed;
    float   m_fTimeToIdle;
    bool    m_bIce;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void Move();

    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);
    virtual void EndOverlap(CCollider3D* _Other);

    CLONE(CPlyWalk);

public:
    CPlyWalk();
    ~CPlyWalk();
};

