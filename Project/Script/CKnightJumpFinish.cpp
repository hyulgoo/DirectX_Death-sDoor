#include "pch.h"
#include "CKnightJumpFinish.h"
#include "CSoundScript.h"

CKnightJumpFinish::CKnightJumpFinish()
{
}

CKnightJumpFinish::~CKnightJumpFinish()
{
}

void CKnightJumpFinish::Enter()
{
	GetOwner()->Animator3D()->Play(6, false);
	CSoundScript* pSoundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = pSoundscript->AddSound(L"Sound\\Monster\\Knight\\KnightJump4.ogg", 1, 0.1f);
}

void CKnightJumpFinish::tick()
{
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"SpinAttack");
	}
}

void CKnightJumpFinish::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
}
