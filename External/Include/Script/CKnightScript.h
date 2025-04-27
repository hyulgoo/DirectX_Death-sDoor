#pragma once
#include <Engine/CScript.h>
#include "CMonsterScript.h"

class CKnightScript :
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

    void SetDirtoPlayer();

    CLONE(CKnightScript);

public:
    CKnightScript();
    ~CKnightScript();
};

