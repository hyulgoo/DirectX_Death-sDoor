#pragma once
#include "CState.h"
class CPlyDodge :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void Dodge(float _fSpeed);

    CLONE(CPlyDodge);

public:
    CPlyDodge();
    ~CPlyDodge();
};

