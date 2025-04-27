#pragma once
#include "CState.h"
class CPlyIdle :
    public CState
{
private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);

    CLONE(CPlyIdle);

public:
    CPlyIdle();
    ~CPlyIdle();
};

