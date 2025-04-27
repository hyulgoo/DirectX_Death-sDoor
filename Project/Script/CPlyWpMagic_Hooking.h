#pragma once
#include "CState.h"
class CPlyWpMagic_Hooking :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpMagic_Hooking);

public:
    CPlyWpMagic_Hooking();
    ~CPlyWpMagic_Hooking();
};

