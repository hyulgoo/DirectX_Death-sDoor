#include "pch.h"
#include "CGrimKnightMelee.h"
#include "CLevelSaveLoadInScript.h"
#include "CPlayerScript.h"
#include "CSoundScript.h"

CGrimKnightMelee::CGrimKnightMelee()
{
}

CGrimKnightMelee::~CGrimKnightMelee()
{
}

void CGrimKnightMelee::Enter()
{
	GetOwner()->Animator3D()->Play(8, false);
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Grim\\GrimaceBite.ogg", 1, 0.1f);
}

void CGrimKnightMelee::tick()
{
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"SpinDown");
	}
}

void CGrimKnightMelee::Exit()
{
	//공격 prefab 생성
	CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\JumpAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.2f);
}
