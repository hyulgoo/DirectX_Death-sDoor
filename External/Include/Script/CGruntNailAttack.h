#pragma once
#include "CState.h"
class CGruntNailAttack :
    public CState
{
private:
    float   m_fTime;
    Vec3    m_Dir;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CGruntNailAttack);

public:
    CGruntNailAttack();
    ~CGruntNailAttack();
};

