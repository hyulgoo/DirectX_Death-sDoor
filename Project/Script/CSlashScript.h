#pragma once
#include <Engine/CScript.h>
class CSlashScript :
    public CScript
{
public:
    virtual void begin() override {};
    virtual void tick() override {};

    virtual void BeginOverlap(CCollider3D* _Other) override;

    CLONE(CSlashScript);

public:
    CSlashScript();
    ~CSlashScript();
};

