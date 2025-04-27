#pragma once
#include <Engine\CScript.h>

class CMonsterDetectRangeScript :
    public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;

    CLONE(CMonsterDetectRangeScript);

public:
    CMonsterDetectRangeScript();
    ~CMonsterDetectRangeScript();
};

