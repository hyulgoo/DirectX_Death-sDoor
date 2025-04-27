#pragma once
#include "CState.h"
class CBazzokaMelee :
    public CState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

    CLONE(CBazzokaMelee);

public:
    CBazzokaMelee();
    ~CBazzokaMelee();
};