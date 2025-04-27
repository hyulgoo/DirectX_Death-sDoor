#include "pch.h"
#include "CPower.h"
#include "CPlayerScript.h"

CPower::CPower()
{
}

CPower::~CPower()
{
}

void CPower::Enter()
{
	GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\BankUI_Power.png", L"texture\\BankUI_Power.png", 0));
}

void CPower::tick()
{
	if (KEY_TAP(KEY::UP))
	{
		ChangeState(L"Magic");
	}
	if (KEY_TAP(KEY::DOWN))
	{
		ChangeState(L"AttackSpeed");
	}
	if (KEY_TAP(KEY::ENTER))
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript<CPlayerScript>()->Upgrade(PLAYER_UPGRADE::ATTACK);
	}
}

void CPower::Exit()
{
}
