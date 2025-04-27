#pragma once
#include <Engine\CScript.h>

class CCrowHeadScript :
    public CScript
{
private:
    CGameObject*    m_pPlayer;
    float           m_fSpeed;

    float m_fLastRenewal;
    float m_fRenewal_Trace; // 추적 경로를 갱신하는 주기
    Vec3  m_vActualPath[256];
    int   m_iActualPathCount;
    int   m_iCurrentPathIndex;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    CLONE(CCrowHeadScript);
public:
    CCrowHeadScript();
    CCrowHeadScript(const CCrowHeadScript& _Other);
    ~CCrowHeadScript();
};

