#pragma once
#include "CState.h"
class CKnightHit :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CKnightHit);

public:
    CKnightHit();
    ~CKnightHit();
};

