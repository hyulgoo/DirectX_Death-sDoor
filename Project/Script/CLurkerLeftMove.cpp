#include "pch.h"
#include "CLurkerLeftMove.h"
#include "CLurkerScript.h"
#include "CSoundScript.h"

void CLurkerLeftMove::Enter()
{
	GetOwner()->Animator3D()->Play(7, true);

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Lurker\\LurkerStep1.ogg", 1, 0.1f);
}

void CLurkerLeftMove::tick()
{
	m_fTime += DT;

	// 플레이어를 기준으로 왼쪽으로 이동.
	Vec3 Velocity = GetOwner()->GetScript<CLurkerScript>()->GetMonsterToPlayerDir();
	float fSpeed = GetOwnerScript()->GetStat().Speed;
	Velocity *= fSpeed;
	swap(Velocity.x, Velocity.z);
	Velocity.z *= -1.f;

	GetOwner()->Rigidbody()->SetVelocity(Velocity);

	// 일정 시간 동안 이동하고 끝.
	if (m_fTime > 1.f)
	{
		GetOwner()->Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
		ChangeState(L"Notify");
	}
}

void CLurkerLeftMove::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_fTime = 0.f;
}

CLurkerLeftMove::CLurkerLeftMove() :
	m_fTime(0.f)
{
}

CLurkerLeftMove::~CLurkerLeftMove()
{
}
