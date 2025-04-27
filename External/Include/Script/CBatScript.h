#pragma once
#include <Engine\CScript.h>
#include "CMonsterScript.h"

class CBatScript    :
    public CMonsterScript
{
private:
    bool m_bRecognizeCheck;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider3D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    bool GetRecognizeCheck() { return m_bRecognizeCheck; }
    CLONE(CBatScript);

public:
    CBatScript();
    ~CBatScript();

};

