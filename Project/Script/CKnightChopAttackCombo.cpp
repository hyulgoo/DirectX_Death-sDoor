#include "pch.h"
#include "CKnightChopAttackCombo.h"
#include "CSoundScript.h"
#include "CGameCameraScript.h"
#include <Engine/CRenderMgr.h>
#include "CKnightScript.h"

CKnightChopAttackCombo::CKnightChopAttackCombo()
	: m_fTime(0.f)
	, m_bCameraShake(false)
{
}

CKnightChopAttackCombo::~CKnightChopAttackCombo()
{
}

void CKnightChopAttackCombo::Enter()
{
	GetOwner()->GetScript<CKnightScript>()->SetDirtoPlayer();
	GetOwner()->Animator3D()->Play(10, false);
	CSoundScript* pSoundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = pSoundscript->AddSound(L"Sound\\Monster\\Knight\\KnightSlam2.ogg", 3, 0.1f);
}

void CKnightChopAttackCombo::tick()
{
	m_fTime += DT;
	float fRatio = m_fTime / GetOwner()->Animator3D()->GetCurClipTimeLength();
	if (!m_bCameraShake && fRatio >= 0.8f)
	{
		CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->CameraShake(10.f, 800.f, 0.1f);
		m_bCameraShake = true;
	}
}

void CKnightChopAttackCombo::Exit()
{
}
