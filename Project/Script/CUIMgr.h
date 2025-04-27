#pragma once
#include <Engine/CSingleton.h>

enum class HUDUI
{
    HP,
    MP,
    END,
};

class CUIMgr :
    public CSingleton<CUIMgr>
{
    SINGLE(CUIMgr);

private:
    CGameObject*    m_arrMagicUI[(UINT)PLAYER_MAGIC::END];
    CGameObject*    m_arrHPMPUI[(UINT)HUDUI::END];
    CGameObject*    m_pFrameUI;
    bool            m_bActive;

public:
    void UpgradePlayer(PLAYER_UPGRADE _tUpgradeType);
    void ActiveMagic(int _tUpgradeType);
    void SetHP(int _iCurHP);
    void SetMP(int _iCurMP, int _iCurMagic);
    void CreateUI();
    
    CLONE(CUIMgr);
};

