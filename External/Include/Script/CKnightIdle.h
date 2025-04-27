#pragma once
#include "CState.h"
class CKnightIdle :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void BeginOverlap(CCollider3D* _Other);

    CLONE(CKnightIdle);

public:
    CKnightIdle();
    ~CKnightIdle();
};

