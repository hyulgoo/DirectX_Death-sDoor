#include "pch.h"
#include "CWaterCameraScript.h"
#include "CGameCameraScript.h"

#include <Engine\CRenderMgr.h>
#include <Engine\CDevice.h>

void CWaterCameraScript::begin()
{
	// MainCamera 참조.
	m_pMainCamera = CRenderMgr::GetInst()->GetMainCam()->GetOwner();

	// MainCamera에서 사용하는 플레이어로의 거리를 가져옴.
	m_vDistance = m_pMainCamera->GetScript<CGameCameraScript>()->GetDistance();

	// 회전은 360 - main cam의 x 회전 값.
	Vec3 CamRot = m_pMainCamera->Transform()->GetRelativeRot();
	CamRot.x = XM_2PI - CamRot.x;
	GetOwner()->Transform()->SetRelativeRot(CamRot);

	// 카메라 스케일은 동일하게.
	float Scale = m_pMainCamera->Camera()->GetScale();
	GetOwner()->Camera()->SetScale(Scale);
}

void CWaterCameraScript::tick()
{
	Vec3 vMainCamPos = m_pMainCamera->Transform()->GetWorldPos();

	vMainCamPos.y = m_fWaterHeight - vMainCamPos.y;

	vMainCamPos.y += m_fYOffset;
	
	GetOwner()->Transform()->SetRelativePos(vMainCamPos);
}

void CWaterCameraScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fWaterHeight, sizeof(float), 1, _File);
	fwrite(&m_fYOffset, sizeof(float), 1, _File);
}

void CWaterCameraScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_fWaterHeight, sizeof(float), 1, _FILE);
	fread(&m_fYOffset, sizeof(float), 1, _FILE);
}

CWaterCameraScript::CWaterCameraScript()
	: CScript((UINT)SCRIPT_TYPE::WATERCAMERASCRIPT)
	, m_pMainCamera(nullptr)
	, m_fWaterHeight(300.f)
	, m_fYOffset(480.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fWaterHeight, "WaterHeight");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fYOffset, "m_YOffset");
}

CWaterCameraScript::~CWaterCameraScript()
{
}
