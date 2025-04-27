#pragma once
#include "CState.h"
class CBazzokaIdle :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CBazzokaIdle);

public:
    CBazzokaIdle();
    ~CBazzokaIdle();
};