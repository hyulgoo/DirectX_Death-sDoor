#include "pch.h"
#include "CCrowBossJump.h"
#include "CCrowBossScript.h"
#include "CCrowBossStomp.h"
#include "CSoundScript.h"

void CCrowBossJump::Enter()
{
	GetOwner()->Animator3D()->Play(9, false);

	m_Dir = GetOwner()->GetScript<CCrowBossScript>()->GetMonsterToPlayerDir();

	m_fDistance = GetOwner()->GetScript<CCrowBossScript>()->GetPlayerDistance();

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\CrowBoss\\OldCrow_Jump2.ogg", 1, 0.1f);
}

void CCrowBossJump::tick()
{
	// 애니메이션 재생시간동안 플레이어의 위치까지 도달하기 위한 Velocity.
	Vec3 Velocity = m_Dir * ((m_fDistance / GetOwner()->Animator3D()->GetCurClipTimeLength()) / 2.f);
	Velocity *= DT;
	float fSpeed = GetOwnerScript()->GetStat().Speed;
	Velocity.y = fSpeed * 30.f * DT;

	GetOwner()->Rigidbody()->AddVelocity(Velocity);

	// 애니메이션이 끝난 경우.
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"Stomp");
		CCrowBossStomp* pCrowBossStomp = (CCrowBossStomp*)GetOwnerScript()->FindState(L"Stomp");
		pCrowBossStomp->SetDir(m_Dir);
		pCrowBossStomp->SetDistance(m_fDistance / 2.f);
	}
}

void CCrowBossJump::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
}

CCrowBossJump::CCrowBossJump()	:
	m_fDistance(0.f)
{
}

CCrowBossJump::~CCrowBossJump()
{
}
