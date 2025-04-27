#pragma once
#include "CState.h"
class CCrowBossCutScene :
    public CState
{
private:
    float   m_fTime;
    bool    m_bCameraShake;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CCrowBossCutScene);

public:
    CCrowBossCutScene();
    ~CCrowBossCutScene();
};

