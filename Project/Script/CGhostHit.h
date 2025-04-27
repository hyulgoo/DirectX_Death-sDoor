#pragma once
#include "CState.h"
class CGhostHit :
    public CState
{
private:
    Vec3 vDir;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGhostHit);

public:
    CGhostHit();
    ~CGhostHit();
};

