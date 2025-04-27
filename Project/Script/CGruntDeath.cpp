#include "pch.h"
#include "CGruntDeath.h"
#include "CGruntScript.h"
#include "CPlayerScript.h"
#include "CSoundScript.h"
#include "CLevelSaveLoadInScript.h"

void CGruntDeath::Enter()
{
	GetOwner()->Animator3D()->Play(13, false);
	GetOwner()->GetScript<CGruntScript>()->SetStarePlayer(false);

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Grunt\\Grunt_DashAttackVoice2.ogg", 1, 0.1f);
}

void CGruntDeath::tick()
{
	GetOwner()->Rigidbody()->ClearForce();
	
	// 애니메이션이 끝난 경우 사망 Paperburn 효과 주기.
	if (GetOwner()->Animator3D()->IsFinish())
	{
		GetOwner()->GetScript<CGruntScript>()->SetPaperBurnEffect(true);
		m_bStartPaperBurn = true;
		GetOwner()->Animator3D()->SetStop(true);
	}

	if (m_bStartPaperBurn)
		m_fPaperBurnTime += DT;

	// 지금까지 흐른 시간이 3초 이상이면 Destory.
	if (m_fPaperBurnTime > 3.f && !GetOwner()->IsDead())
	{
		GetOwner()->GetScript<CGruntScript>()->GetPlayer()->GetScript<CPlayerScript>()->AddMoney((UINT)500);
		CLevelSaveLoadInScript LSL;
		LSL.MoneyCount((UINT)500);
		GetOwnerScript()->Destroy();
	}
}

void CGruntDeath::Exit()
{
}

CGruntDeath::CGruntDeath() :
	m_bStartPaperBurn(false)
	, m_fPaperBurnTime(0.f)
{
}

CGruntDeath::~CGruntDeath()
{
}
