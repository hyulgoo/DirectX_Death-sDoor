#include "pch.h"
#include "CBankUpgradeScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CPower.h"
#include "CAttackSpeed.h"
#include "CSpeed.h"
#include "CMagic.h"
#include "CStateScript.h"
#include <Engine/CEventMgr.h>
#include "CStateScript.h"
#include "CPlayerScript.h"
#include "Engine/CEngine.h"

CBankUpgradeScript::CBankUpgradeScript()	
	: CScript(SCRIPT_TYPE::BANKUPGRADESCRIPT)
	, m_pPlayer(nullptr)
	, m_pStateScript(nullptr)
{
}

CBankUpgradeScript::~CBankUpgradeScript()
{
}

void CBankUpgradeScript::begin()
{
	int iMtrlCount = MeshRender()->GetMtrlCount();

	for (int i = 0; i < iMtrlCount; ++i)
	{
		MeshRender()->GetDynamicMaterial(i);
	}

	// 상태 설정
	if (nullptr == m_pStateScript)
	{
		m_pStateScript = GetOwner()->GetScript<CStateScript>();
		m_pStateScript->AddState(L"Power", new CPower);
		m_pStateScript->AddState(L"AttackSpeed", new CAttackSpeed);
		m_pStateScript->AddState(L"Speed", new CSpeed);
		m_pStateScript->AddState(L"Magic", new CMagic);

		m_pStateScript->ChangeState(L"Power");
	}
	GetOwner()->Transform()->SetRelativeScale(Vec3(1682.f, 980.f, 1.f));
}

void CBankUpgradeScript::tick()
{

	if (KEY_TAP(KEY::ESC))
	{
		Destroy();
	}
}

void CBankUpgradeScript::BeginOverlap(CCollider3D* _Other)
{
}

void CBankUpgradeScript::OnOverlap(CCollider3D* _Other)
{
	
}

void CBankUpgradeScript::EndOverlap(CCollider3D* _Other)
{
}

void CBankUpgradeScript::Upgrade(PLAYER_UPGRADE _tUpgradeType)
{
	m_pPlayer->GetScript<CPlayerScript>()->Upgrade(_tUpgradeType);
}
