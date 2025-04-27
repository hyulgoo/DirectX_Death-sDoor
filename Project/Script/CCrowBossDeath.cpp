#include "pch.h"
#include "CCrowBossDeath.h"
#include "CCrowBossScript.h"
#include "CPlayerScript.h"
#include "CSoundScript.h"
#include "CLevelSaveLoadInScript.h"

void CCrowBossDeath::Enter()
{
	GetOwner()->Animator3D()->Play(7, false);
	GetOwner()->GetScript<CCrowBossScript>()->SetStarePlayer(false);
	GetOwner()->GetScript<CCrowBossScript>()->DestoryFeather();

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\CrowBoss\\OldCrow_Death.ogg", 1, 0.1f);
}

void CCrowBossDeath::tick()
{
	GetOwner()->Rigidbody()->ClearForce();
	
	// 애니메이션이 끝난 경우 사망 Paperburn 효과 주기.
	if (GetOwner()->Animator3D()->IsFinish())
	{
		GetOwner()->GetScript<CCrowBossScript>()->SetPaperBurnEffect(true);
		m_bStartPaperBurn = true;
		GetOwner()->Animator3D()->SetStop(true);
	}

	if (m_bStartPaperBurn)
		m_fPaperBurnTime += DT;

	// 지금까지 흐른 시간이 3초 이상이면 Destory.
	if (m_fPaperBurnTime > 3.f && !GetOwner()->IsDead())
	{
		GetOwner()->GetScript<CCrowBossScript>()->GetPlayer()->GetScript<CPlayerScript>()->AddMoney((UINT)30000);
		CLevelSaveLoadInScript LSL;
		LSL.MoneyCount((UINT)30000);
		GetOwnerScript()->Destroy();

		CLevelMgr::GetInst()->FindObjectByName(L"Player")->GetScript<CPlayerScript>()->ChangeState(L"Dance");
	}
}

void CCrowBossDeath::Exit()
{
}

CCrowBossDeath::CCrowBossDeath() :
	m_bStartPaperBurn(false)
	, m_fPaperBurnTime(0.f)
{
}

CCrowBossDeath::~CCrowBossDeath()
{
}
