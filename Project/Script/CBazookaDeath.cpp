#include "pch.h"
#include "CBazookaDeath.h"
#include "CBazookaScript.h"
#include "CPlayerScript.h"
#include "CSoundScript.h"
#include "CLevelSaveLoadInScript.h"

void CBazookaDeath::Enter()
{
	GetOwner()->GetScript<CBazookaScript>()->SetPaperBurnEffect(true);
	GetOwner()->GetScript<CBazookaScript>()->SetStarePlayer(false);
	// 몬스터 사망시 현재까지 흐른 시간을 저장.
	m_bStartPaperBurn = true;
	GetOwner()->Animator3D()->SetStop(true);

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Bazooka\\PlagueBoyDeath.ogg", 1, 0.1f);
}

void CBazookaDeath::tick()
{
	GetOwner()->Rigidbody()->ClearForce();
	
	if (m_bStartPaperBurn)
		m_fPaperBurnTime += DT;

	// 지금까지 흐른 시간이 3초 이상이면 Destory.
	if (m_fPaperBurnTime > 3.f && !GetOwner()->IsDead())
	{
		GetOwner()->GetScript<CBazookaScript>()->GetPlayer()->GetScript<CPlayerScript>()->AddMoney((UINT)600);
		CLevelSaveLoadInScript LSL;
		LSL.MoneyCount((UINT)600);
		GetOwnerScript()->Destroy();
	}
}

void CBazookaDeath::Exit()
{
}

CBazookaDeath::CBazookaDeath() :
	m_bStartPaperBurn(false)
	, m_fPaperBurnTime(0.f)
{
}

CBazookaDeath::~CBazookaDeath()
{
}
