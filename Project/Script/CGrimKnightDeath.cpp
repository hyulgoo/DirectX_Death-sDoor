#include "pch.h"
#include "CGrimKnightDeath.h"
#include "CGrimKnightScript.h"
#include "CSoundScript.h"
#include "CPlayerScript.h"
#include "CLevelSaveLoadInScript.h"

CGrimKnightDeath::CGrimKnightDeath()		:
	m_bStartPaperBurn(false),
	m_fPaperBurnTime(0.f)
{
}

CGrimKnightDeath::~CGrimKnightDeath()
{
}

void CGrimKnightDeath::tick()
{
	GetOwner()->Rigidbody()->ClearForce();
	if (m_bStartPaperBurn)
		m_fPaperBurnTime += DT;

	// 지금까지 흐른 시간이 3초 이상이면 Destory.
	if (m_fPaperBurnTime > 3.f && GetOwner()->IsDead() == false)
	{
		GetOwner()->GetScript<CGrimKnightScript>()->GetPlayer()->GetScript<CPlayerScript>()->AddMoney((UINT)600);
		CLevelSaveLoadInScript script;
		script.MoneyCount(600);
		GetOwnerScript()->Destroy();
	}
}

void CGrimKnightDeath::Enter()
{
	GetOwner()->GetScript<CGrimKnightScript>()->SetPaperBurnEffect(true);
	// 몬스터 사망시 현재까지 흐른 시간을 저장.
	m_bStartPaperBurn = true;

	CSoundScript* pSoundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = pSoundscript->AddSound(L"Sound\\Monster\\Grim\\GrimaceLastHit.ogg", 1, 0.1f);
}

void CGrimKnightDeath::Exit()
{
}
