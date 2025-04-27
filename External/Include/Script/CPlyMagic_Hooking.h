#pragma once
#include "CState.h"
class CPlyMagic_Hooking :
    public CState
{
private:
    CGameObject*            m_pHook;
    vector<CGameObject*>    m_vecChain;
    Vec3                    m_vHookPos;
    Vec3                    m_vDir;

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
    void SetHookedPos(Vec3 _vHookedPos) { m_vHookPos = _vHookedPos; }
    void SetHook(CGameObject* _pHookObj) 
    { 
        if(nullptr == m_pHook)
            m_pHook = _pHookObj; 
    }
    void SetChain(vector<CGameObject*>& _vecChain) 
    { 
        if(m_vecChain.empty())
            m_vecChain = _vecChain; 
    }


    CLONE(CPlyMagic_Hooking);

public:
    CPlyMagic_Hooking();
    ~CPlyMagic_Hooking();
    
    friend class CPlyMagic_Hook;
};

