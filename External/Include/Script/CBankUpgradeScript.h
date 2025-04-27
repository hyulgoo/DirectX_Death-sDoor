#pragma once
#include <Engine\CScript.h>
#include "CStateScript.h"

class CBankUpgradeScript :
	public CScript
{
private:
    CGameObject*    m_pPlayer;
    CStateScript*   m_pStateScript;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;
    void Upgrade(PLAYER_UPGRADE _tUpgradeType);

    CLONE(CBankUpgradeScript);
public:
    CBankUpgradeScript();
    ~CBankUpgradeScript();
};

