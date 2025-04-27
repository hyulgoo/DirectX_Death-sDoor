#include "pch.h"
#include "CKnightCutScene.h"
#include "CGameCameraScript.h"
#include <Engine/CRenderMgr.h>

CKnightCutScene::CKnightCutScene()
	: m_fTime(0.f)
	, m_bCameraShake(false)
{
}

CKnightCutScene::~CKnightCutScene()
{
}

void CKnightCutScene::Enter()
{
	GetOwner()->Animator3D()->Play(4, false);
}

void CKnightCutScene::tick()
{
	m_fTime += DT;
	float fRatio = m_fTime / GetOwner()->Animator3D()->GetCurClipTimeLength();
	if (!m_bCameraShake && fRatio >= 0.8f)
	{
		CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->CameraShake(10.f, 800.f, 0.1f);
		m_bCameraShake = true;
	}
}

void CKnightCutScene::Exit()
{
}
