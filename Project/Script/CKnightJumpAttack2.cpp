#include "pch.h"
#include "CKnightJumpAttack2.h"
#include "CLevelSaveLoadInScript.h"
#include "CSoundScript.h"
#include "CKnightScript.h"

CKnightJumpAttack2::CKnightJumpAttack2()
	: m_fDelay(0.f)
	, m_bAttack(false)
{
}

CKnightJumpAttack2::~CKnightJumpAttack2()
{
}

void CKnightJumpAttack2::Enter()
{
	GetOwner()->Animator3D()->Play(14, false);
}

void CKnightJumpAttack2::tick()
{
	m_fDelay += DT;
	float fRatio = m_fDelay / GetOwner()->Animator3D()->GetCurClipTimeLength();
	if (fRatio > 0.8f && !m_bAttack)
	{
		CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\JumpAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.2f);
		CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
		Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Knight\\KnightSlam1.ogg", 1, 0.1f);
		m_bAttack = true;
	}

	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"JumpFinish");
	}
}

void CKnightJumpAttack2::Exit()
{
	m_fDelay = 0.f;
	m_bAttack = false;
	GetOwner()->Rigidbody()->ClearForce();
}
