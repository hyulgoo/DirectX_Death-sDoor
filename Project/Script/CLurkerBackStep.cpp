#include "pch.h"
#include "CLurkerBackStep.h"
#include "CLurkerScript.h"
#include "CSoundScript.h"

void CLurkerBackStep::Enter()
{
	GetOwner()->Animator3D()->Play(4, false);
	m_Dir = GetOwner()->GetScript<CLurkerScript>()->GetMonsterToPlayerDir();
	GetOwner()->Rigidbody()->SetVelocityLimit(800.f);

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Lurker\\LurkerDodgeWoosh1.ogg", 1, 0.1f);
}

void CLurkerBackStep::tick()
{
	float AnimLength = (float)GetOwner()->Animator3D()->GetCurClipTimeLength();
	m_fTime += DT;
	float CurRatio = m_fTime / AnimLength;

	if (CurRatio <= 0.5f)
	{
		Vec3 Velocity = m_Dir;
		float fSpeed = GetOwnerScript()->GetStat().Speed;
		Velocity *= fSpeed * -8.f * DT;
		Velocity.y = 20.f * DT;

		GetOwner()->Rigidbody()->AddVelocity(Velocity);
	}

	// 애니메이션이 끝나면 Notify로 다시 변경.
	if (GetOwner()->Animator3D()->IsFinish())
		ChangeState(L"Notify");
}

void CLurkerBackStep::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_fTime = 0.f;

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Lurker\\LurkerLand1.ogg", 1, 0.1f);
}

CLurkerBackStep::CLurkerBackStep() :
	m_fTime(0.f)
{
}

CLurkerBackStep::~CLurkerBackStep()
{
}
