#include "pch.h"
#include "CPlyWpDance.h"
#include <Engine/CRenderMgr.h>
#include "CGameCameraScript.h"

CPlyWpDance::CPlyWpDance()
{
}

CPlyWpDance::~CPlyWpDance()
{
}

void CPlyWpDance::Enter()
{
	GetOwner()->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::DANCE, false);
}

void CPlyWpDance::tick()
{
	m_fCameraZoomIn += DT;

	if(m_fCameraZoomIn <= 5.f)
	{
		CRenderMgr::GetInst()->GetMainCam()->SetScale(0.6f + 0.6f * (m_fCameraZoomIn / 5.f));
		CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->SetCutSceneView(true);
	}
}

void CPlyWpDance::Exit()
{
	GetOwner()->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
}
