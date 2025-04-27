#pragma once
#include <Engine/CEntity.h>
#include "CStateScript.h"

class CState :
    public CEntity
{
private:
    CStateScript* m_pOwnerScript;

public:
    virtual void tick() = 0;
    virtual void Enter() = 0; // 새로운 state로 변경될때 호출 됨.
    virtual void Exit() = 0;  // 다른 state로 변경되기 직전에 호출 됨.

public:
    void ChangeState(wstring _strStateName);

    CStateScript* GetOwnerScript() { return m_pOwnerScript; }
    CGameObject* GetOwner() { return m_pOwnerScript->GetOwner(); }

    virtual void BeginOverlap(CCollider3D* _Other) {};
    virtual void OnOverlap(CCollider3D* _Other) {};
    virtual void EndOverlap(CCollider3D* _Other) {};

public:
    CState* Clone() = 0;

    CState();
    ~CState();

    friend class CStateScript;
};

