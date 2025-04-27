#pragma once
#include <Engine/CScript.h>
class CMainLightScript :
    public CScript
{
private:
    CGameObject* m_pPlayer;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMainLightScript);

    CMainLightScript();
    ~CMainLightScript();
};

