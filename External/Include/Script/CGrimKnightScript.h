#pragma once
#include<Engine/CScript.h>
#include "CMonsterScript.h"
class CGrimKnightScript :
    public CMonsterScript
{
private:
    bool m_bRecognizeCheck;
    bool m_bRetrace;
    bool m_bOnCollision;
    int  m_iHitCount;

public:
    void SetOnCollision(bool onCol) { m_bOnCollision = onCol; }
    int GetHitCount() { return m_iHitCount; }
public:
    virtual void begin() override;
    virtual void tick() override;
    void CalcDir();

public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CGrimKnightScript);

public:
    CGrimKnightScript();
    ~CGrimKnightScript();
};

