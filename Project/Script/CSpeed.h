#pragma once
#include "CState.h"
class CSpeed :
    public CState
{
private:
    bool m_bCheck;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    bool GetSpeedCheck() { return m_bCheck; }
    void SetSpeedCheck(bool _check) { m_bCheck = _check; }
    CLONE(CSpeed);

public:
    CSpeed();
    ~CSpeed();
};

