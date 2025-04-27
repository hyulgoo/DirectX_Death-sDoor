#pragma once
#include "CState.h"
class CLurkerNotify :
    public CState
{
private:
    bool    m_bNotifyAnimEnd;
    int     m_iPrevPattern;
    bool    m_bJustBeforeBackStep;  // 방금 전에 BackStep을 한 경우.

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    CLONE(CLurkerNotify);

public:
    CLurkerNotify();
    ~CLurkerNotify();
};

