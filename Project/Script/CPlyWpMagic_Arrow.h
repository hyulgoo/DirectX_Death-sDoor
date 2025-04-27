#pragma once
#include "CState.h"
class CPlyWpMagic_Arrow :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpMagic_Arrow);

public:
    CPlyWpMagic_Arrow();
    ~CPlyWpMagic_Arrow();
};

