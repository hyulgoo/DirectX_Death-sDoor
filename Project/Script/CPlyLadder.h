#pragma once
#include "CState.h"
class CPlyLadder :
    public CState
{
private:
    float   m_fSpeed;
    float   m_fStartYPos;
    float   m_fLadderHeight;
    float   m_fStartDelay;
    float   m_fGroundCheckDelay;
    bool    m_bEnd;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    void Move();
    void SetHeight(float _fHeight) { m_fLadderHeight = _fHeight; }

    CLONE(CPlyLadder);

public:
    CPlyLadder();
    ~CPlyLadder();
};

