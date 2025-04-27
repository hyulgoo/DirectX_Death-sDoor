#pragma once
#include "CState.h"
class CGruntDeath :
    public CState
{
private:
    bool    m_bStartPaperBurn;
    float   m_fPaperBurnTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntDeath);

public:
    CGruntDeath();
    ~CGruntDeath();
};

