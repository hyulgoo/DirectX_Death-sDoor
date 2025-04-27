#include "pch.h"
#include "CCrowBossIdle.h"
#include "CCrowBossScript.h"

void CCrowBossIdle::Enter()
{
	GetOwner()->Animator3D()->Play(0, true);
}

void CCrowBossIdle::tick()
{
	CGameObject* pPlayer = GetOwner()->GetScript<CCrowBossScript>()->GetPlayer();

	// Detect 상태라면 Notify 상태로 전환.
	if (1000.f > abs((pPlayer->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos()).Length()))
	{
		ChangeState(L"CutScene");
	}
}

void CCrowBossIdle::Exit()
{
}

CCrowBossIdle::CCrowBossIdle()
{
}

CCrowBossIdle::~CCrowBossIdle()
{
}
