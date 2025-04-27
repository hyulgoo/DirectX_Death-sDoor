#include "pch.h"
#include "CMagic.h"
#include "CPlayerScript.h"

CMagic::CMagic()
{
}

CMagic::~CMagic()
{
}

void CMagic::Enter()
{
	GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\BankUI_Magic.png", L"texture\\BankUI_Magic.png", 0));
}

void CMagic::tick()
{
	if (KEY_TAP(KEY::UP))
	{
		ChangeState(L"Speed");
	}
	if (KEY_TAP(KEY::DOWN))
	{
		ChangeState(L"Power");
	}
	if (KEY_TAP(KEY::ENTER))
	{
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript<CPlayerScript>()->Upgrade(PLAYER_UPGRADE::MAGIC);
	}
}

void CMagic::Exit()
{
}
