#pragma once
#include "CState.h"
class CPower :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPower);

public:
    CPower();
    ~CPower();
};

