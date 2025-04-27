#pragma once
#include "CState.h"
class CPlyHit :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void BeginOverlap(CCollider3D* _Other);
    virtual void OnOverlap(CCollider3D* _Other);
    virtual void EndOverlap(CCollider3D* _Other);

    CLONE(CPlyHit);

public:
    CPlyHit();
    ~CPlyHit();
};

