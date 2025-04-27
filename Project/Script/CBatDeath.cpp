#include "pch.h"
#include "CBatDeath.h"
#include "CBatScript.h"
#include "CPlayerScript.h"
#include "CLevelSaveLoadInScript.h"


CBatDeath::CBatDeath()	
	: m_fPaperBurnTime(0.f)
{
}

CBatDeath::~CBatDeath()
{
}

void CBatDeath::Enter()
{
	GetOwner()->GetScript<CBatScript>()->SetPaperBurnEffect(true);
	// 몬스터 사망시 현재까지 흐른 시간을 저장.
}

void CBatDeath::tick()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_fPaperBurnTime += DT;

	// 지금까지 흐른 시간이 3초 이상이면 Destory.
	if (m_fPaperBurnTime > 3.f)
	{
		GetOwner()->GetScript<CBatScript>()->GetPlayer()->GetScript<CPlayerScript>()->AddMoney((UINT)100);
		CLevelSaveLoadInScript script;
		script.MoneyCount(100);
		GetOwnerScript()->Destroy();
	}
		
}

void CBatDeath::Exit()
{
}
