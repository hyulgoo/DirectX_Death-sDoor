#pragma once
#include "CState.h"
class CBatIdle :
    public CState
{
private:
    float m_fTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CBatIdle);

public:
    CBatIdle();
    ~CBatIdle();
};

