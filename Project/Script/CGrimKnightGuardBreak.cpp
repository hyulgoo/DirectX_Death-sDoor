#include "pch.h"
#include "CGrimKnightGuardBreak.h"
#include "CSoundScript.h"

CGrimKnightGuardBreak::CGrimKnightGuardBreak()
{
}

CGrimKnightGuardBreak::~CGrimKnightGuardBreak()
{
}

void CGrimKnightGuardBreak::tick()
{
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"BackStep1");
	}

	GetOwner()->Rigidbody()->ClearForce();
}

void CGrimKnightGuardBreak::Enter()
{
	GetOwner()->Animator3D()->Play(3, false);
	CSoundScript* pSoundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = pSoundscript->AddSound(L"Sound\\Monster\\Grim\\GrimaceShieldBreak.ogg", 1, 0.1f);
}

void CGrimKnightGuardBreak::Exit()
{
}
