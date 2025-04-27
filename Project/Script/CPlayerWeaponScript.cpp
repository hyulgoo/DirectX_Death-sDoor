#include "pch.h"
#include "CPlayerWeaponScript.h"
#include "CStateScript.h"
#include "PlayerWeaponStates.h"

CPlayerWeaponScript::CPlayerWeaponScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERWEAPONSCRIPT)
	, m_pStateScript(nullptr)
{
}

CPlayerWeaponScript::~CPlayerWeaponScript()
{
}

void CPlayerWeaponScript::begin()
{
	if (nullptr == m_pStateScript)
	{
		m_pStateScript = GetOwner()->GetScript<CStateScript>();
		m_pStateScript->AddState(L"Idle", new CPlyWpIdle);
		m_pStateScript->AddState(L"Walk", new CPlyWpWalk);
		m_pStateScript->AddState(L"Run", new CPlyWpRun);
		m_pStateScript->AddState(L"Dodge", new CPlyWpDodge);
		m_pStateScript->AddState(L"Dance", new CPlyWpDance);
		m_pStateScript->AddState(L"Fall", new CPlyWpFall);
		m_pStateScript->AddState(L"Hit", new CPlyWpHit);
		m_pStateScript->AddState(L"Dead", new CPlyWpDead);
		m_pStateScript->AddState(L"Ladder", new CPlyWpLadder);
		m_pStateScript->AddState(L"Attack", new CPlyWpAttack);
		m_pStateScript->AddState(L"Arrow", new CPlyWpMagic_Arrow);
		m_pStateScript->AddState(L"Fire", new CPlyWpMagic_Fire);
		m_pStateScript->AddState(L"Bomb", new CPlyWpMagic_Bomb);
		m_pStateScript->AddState(L"Hook", new CPlyWpMagic_Hook);
		m_pStateScript->AddState(L"Hooking", new CPlyWpMagic_Hooking);
		m_pStateScript->ChangeState(L"Idle");
		MeshRender()->GetDynamicMaterial(0);
	}
}

void CPlayerWeaponScript::tick()
{
}

void CPlayerWeaponScript::BeginOverlap(CCollider3D* _Other)
{
}

void CPlayerWeaponScript::EndOverlap(CCollider3D* _Other)
{
}

void CPlayerWeaponScript::ChangeState(wstring _strStateName)
{
	m_pStateScript->ChangeState(_strStateName);
}

void CPlayerWeaponScript::SaveToLevelFile(FILE* _File)
{
}

void CPlayerWeaponScript::LoadFromLevelFile(FILE* _FILE)
{
}
