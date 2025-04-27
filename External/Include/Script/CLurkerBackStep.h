#pragma once
#include "CState.h"
class CLurkerBackStep :
    public CState
{
private:
    float   m_fTime;
    Vec3    m_Dir;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    CLONE(CLurkerBackStep);

public:
    CLurkerBackStep();
    ~CLurkerBackStep();
};

