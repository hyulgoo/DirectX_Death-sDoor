#pragma once
#include <Engine\CScript.h>
class CBazookaGasGrenadeScript :
    public CScript
{
private:
    Vec3            m_vShotDir;
    Vec3            m_vPrevVelocity;
    int             m_iState;       // 0: 날아가는 중. 1: 땅에 떨어져서 퍼지는 중.
    CGameObject*    m_pGasBulletParticle;
    CGameObject*    m_pGasCenterParticle;
    CGameObject*    m_pGasRoundParticle;
    float           m_fBulletTime;
    float           m_fGasTime;

public:
    void SetShotDir(Vec3 _ShotDir) { m_vShotDir = _ShotDir; }
    Vec3 GetShotDir() { return m_vShotDir; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    CLONE(CBazookaGasGrenadeScript);
public:
    CBazookaGasGrenadeScript();
    CBazookaGasGrenadeScript(const CBazookaGasGrenadeScript& _Other);
    ~CBazookaGasGrenadeScript();
};

