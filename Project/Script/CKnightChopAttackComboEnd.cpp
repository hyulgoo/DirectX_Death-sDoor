#include "pch.h"
#include "CKnightChopAttackComboEnd.h"
#include "CGameCameraScript.h"
#include <Engine/CRenderMgr.h>
#include "CKnightScript.h"

CKnightChopAttackComboEnd::CKnightChopAttackComboEnd()
	: m_fTime(0.f)
	, m_bCameraShake(false)
{
}

CKnightChopAttackComboEnd::~CKnightChopAttackComboEnd()
{
}

void CKnightChopAttackComboEnd::Enter()
{
	GetOwner()->GetScript<CKnightScript>()->SetDirtoPlayer();
	GetOwner()->Animator3D()->Play(11, false);
}

void CKnightChopAttackComboEnd::tick()
{
	m_fTime += DT;
	float fRatio = m_fTime / GetOwner()->Animator3D()->GetCurClipTimeLength();
	if (!m_bCameraShake && fRatio >= 0.8f)
	{
		CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->CameraShake(10.f, 800.f, 0.1f);
		m_bCameraShake = true;
	}
}

void CKnightChopAttackComboEnd::Exit()
{
}
