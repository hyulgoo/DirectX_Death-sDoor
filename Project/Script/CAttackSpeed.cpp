#include "pch.h"
#include "CAttackSpeed.h"
#include "CPlayerScript.h"

CAttackSpeed::CAttackSpeed()
{
}

CAttackSpeed::~CAttackSpeed()
{
}

void CAttackSpeed::Enter()
{
	GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\BankUI_AttackSpeed.png", L"texture\\BankUI_AttackSpeed.png", 0));
}

void CAttackSpeed::tick()
{
	if (KEY_TAP(KEY::UP))
	{
		ChangeState(L"Power");
	}
	if (KEY_TAP(KEY::DOWN))
	{
		ChangeState(L"Speed");
	}

	if (KEY_TAP(KEY::ENTER))
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript<CPlayerScript>()->Upgrade(PLAYER_UPGRADE::ATK_SPEED);
	}
}

void CAttackSpeed::Exit()
{
}
