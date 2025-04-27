#pragma once
#include "CState.h"
class CMagic :
    public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CMagic);

public:
    CMagic();
    ~CMagic();
};

