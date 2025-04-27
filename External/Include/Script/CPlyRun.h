#pragma once
#include "CState.h"
class CPlyRun :
    public CState
{
private:
    float   m_fSpeed; 
    float   m_fTimeToIdle;
    float   m_fRotDelay;
    float   m_fRot;
    bool    m_bIce;
    float   m_fStepSoundDelay;
    bool    m_bStepSoundOrder;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void Move();

    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);

    CLONE(CPlyRun);

public:
    CPlyRun();
    ~CPlyRun();
};

