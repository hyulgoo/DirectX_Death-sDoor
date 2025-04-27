#pragma once
#include "CMonsterScript.h"
class CGhostScript :
    public CMonsterScript
{
private:
    bool m_bRecognizeCheck;
    bool m_bOnCollision;
    bool m_bIsHit;
    Vec3 m_vPlayerPos;
public:
    virtual void begin() override;
    virtual void tick() override;
private:
    Vec3 GetDir(Vec3 _start, Vec3 _target);
public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CGhostScript);

public:
    CGhostScript();
    CGhostScript(const CGhostScript& _Other);
    ~CGhostScript();
};

