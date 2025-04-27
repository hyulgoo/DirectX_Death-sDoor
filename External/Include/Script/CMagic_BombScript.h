#pragma once
#include <Engine/CScript.h>
class CMagic_BombScript :
    public CScript
{
private:
    float   m_fDamage;
    float   m_fSpeed;
    Vec3    m_vDir;
    bool    m_bThrow;
    float   m_fPrevDirRatio;
    bool    m_bCollided;
    float   m_fAffterCollided;
    bool    m_bCollidable;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetDamege(float _fDamege) { m_fDamage = _fDamege; }
    void SetDir(Vec3 _vDir) {
        m_vDir = _vDir.Normalize(); 
        m_vDir.y = 0.f; 
        m_fPrevDirRatio = m_vDir.x / m_vDir.z;
    }
    void SetThrow() { m_bThrow = true; }
    void SetCollidable() { m_bCollidable = true; }

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CMagic_BombScript);

public:
    CMagic_BombScript();
    ~CMagic_BombScript();
};

