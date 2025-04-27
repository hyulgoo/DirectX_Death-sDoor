#include "pch.h"
#include "CSpeed.h"
#include "CPlayerScript.h"

CSpeed::CSpeed()	:
	m_bCheck(false)
{

}

CSpeed::~CSpeed()
{
}

void CSpeed::Enter()
{

	GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\BankUI_Speed.png", L"texture\\BankUI_Speed.png", 0));
}

void CSpeed::tick()
{
	if (KEY_TAP(KEY::UP))
	{
		ChangeState(L"AttackSpeed");
	}
	if (KEY_TAP(KEY::DOWN))
	{
		ChangeState(L"Magic");
	}
	if (KEY_TAP(KEY::ENTER))
	{
		int currentMoney = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript<CPlayerScript>()->GetMoneyCount();
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript<CPlayerScript>()->AddMoney(-600);
		CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript<CPlayerScript>()->Upgrade(PLAYER_UPGRADE::SPEED);
		m_bCheck = true;
	}
}

void CSpeed::Exit()
{
}
