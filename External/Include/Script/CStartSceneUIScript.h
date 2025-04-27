#pragma once
#include <Engine/CScript.h>
class CStartSceneUIScript :
    public CScript
{
public:
    virtual void tick() override;

    CLONE(CStartSceneUIScript);
public:
    CStartSceneUIScript();
    ~CStartSceneUIScript();
};

