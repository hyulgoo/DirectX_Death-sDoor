#pragma once
#include "CState.h"
class CLurkerAttack:
    public CState
{
private:
    float   m_fTime;
    Vec3    m_Dir;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    CLONE(CLurkerAttack);

public:
    CLurkerAttack();
    ~CLurkerAttack();
};

