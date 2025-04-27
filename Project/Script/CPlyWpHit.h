#pragma once
#include "CState.h"
class CPlyWpHit :
    public CState
{
private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CPlyWpHit);

public:
    CPlyWpHit();
    ~CPlyWpHit();
};

