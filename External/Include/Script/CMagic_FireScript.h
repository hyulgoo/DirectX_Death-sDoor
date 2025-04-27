#pragma once
#include <Engine/CScript.h>
class CMagic_FireScript :
    public CScript
{
private:
    Vec3    m_vStartPos;
    Vec3    m_vDir;
    float   m_fDamage;
    float   m_fSpeed;
    bool    m_bCollidable;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetDamege(float _fDamege) { m_fDamage = _fDamege; }
    void SetDir(Vec3 _vDir) { m_vDir = _vDir.Normalize(); m_vDir.y = 0.f; }
    void SetStartPos(Vec3 _vPos) { m_vStartPos = _vPos; }
    void SetCollidable() { m_bCollidable = true; }

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CMagic_FireScript);

public:
    CMagic_FireScript();
    ~CMagic_FireScript();
};

