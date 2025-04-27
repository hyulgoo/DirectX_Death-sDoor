#include "pch.h"
#include "CKnightChopAttack.h"
#include "CSoundScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CGameCameraScript.h"
#include <Engine/CRenderMgr.h>
#include "CKnightScript.h"

CKnightChopAttack::CKnightChopAttack()
	: m_fTime(0.f)
	, m_bCameraShake(false)
{
}

CKnightChopAttack::~CKnightChopAttack()
{
}

void CKnightChopAttack::Enter()
{
	GetOwner()->GetScript<CKnightScript>()->SetDirtoPlayer();
	GetOwner()->Animator3D()->Play(8, false);
	CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\JumpAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.2f);
	CSoundScript* pSoundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = pSoundscript->AddSound(L"Sound\\Monster\\Knight\\KnightSlam1.ogg", 1, 0.1f);
}

void CKnightChopAttack::tick()
{
	m_fTime += DT;
	float fRatio = m_fTime / GetOwner()->Animator3D()->GetCurClipTimeLength();
	if (!m_bCameraShake && fRatio >= 0.8f)
	{
		CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->CameraShake(10.f, 800.f, 0.1f);
		m_bCameraShake = true;
	}
}

void CKnightChopAttack::Exit()
{
}
