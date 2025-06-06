#include "pch.h"
#include "CLight3D.h"

#include "CCamera.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CResMgr.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightInfo{}
	, m_iLightIdx(0)
{
	SetLightType(LIGHT_TYPE::POINT);

	m_pLightCam = new CGameObject;
	m_pLightCam->AddComponent(new CTransform);
	m_pLightCam->AddComponent(new CCamera);
}

CLight3D::CLight3D(const CLight3D& _origin)
	: CComponent(_origin)
	, m_LightInfo(_origin.m_LightInfo)
	, m_iLightIdx(-1)
	, m_pLightCam(nullptr)
{
	m_pLightCam = _origin.m_pLightCam->Clone();
}

CLight3D::~CLight3D()
{
	if (nullptr != m_pLightCam)
		delete m_pLightCam;
}


void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_LightInfo.LightType = (UINT)_Type;

	if (LIGHT_TYPE::DIRECTIONAL == _Type)
	{
		// Directional Light는 전역을 비추기 때문에 화면 전체를 호출해야 하므로
		// Local 정점 위치를 확장하면 화면 전체 픽셀을 호출할 수 있는 RectMesh를 사용
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_LightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl"); 

		m_pLightCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pLightCam->Camera()->SetWidth(10600.f);
		m_pLightCam->Camera()->SetAspectRatio(1.f);
		m_pLightCam->Camera()->SetFar(50000.f);
	}

	else if (LIGHT_TYPE::POINT == _Type)
	{
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_LightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl"); 
	}

	else
	{
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_LightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"SpotLightMtrl");
	}
}

void CLight3D::SetLightDirection(Vec3 _vDir)
{
	// 받은 방향을 정규화
	m_LightInfo.vWorldDir = _vDir;
	m_LightInfo.vWorldDir.Normalize();

	// 방향을 z축으로 삼아 광원 기준 축 계산
	Vec3 vFront = m_LightInfo.vWorldDir;
	Vec3 vUp = Vec3(0.f, 1.f, 0.f);
	Vec3 vRight = XMVector3Cross(vUp, vFront);
	vRight.Normalize();
	vUp = XMVector3Cross(vFront, vRight);
	vUp.Normalize();

	// 얻은 축을 회전행렬로 만듦
	Matrix matRot = XMMatrixIdentity();
	matRot._11 = vRight.x;	matRot._12 = vRight.y;	matRot._13 = vRight.z;
	matRot._21 = vUp.x;		matRot._22 = vUp.y;		matRot._23 = vUp.z;
	matRot._31 = vFront.x;	matRot._32 = vFront.y;	matRot._33 = vFront.z;

	// 만든 회전행렬로 축별 회전 정도를 Vec3로 얻어냄
	Vec3 vRot = DecomposeRotMat(matRot);

	// 광원이 가리키는 방향이 Transform 의 Front 가 되도록 회전시켜준다.
	Transform()->SetRelativeRot(vRot);
}

void CLight3D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Transform()->SetRelativeScale(Vec3(m_LightInfo.Radius * 2.f, m_LightInfo.Radius * 2.f, m_LightInfo.Radius * 2.f));

	// 광원의 카메라도 광원과 동일한 Transform 이 되도록 업데이트 한다.
	m_pLightCam->Transform()->SetRelativePos(Transform()->GetWorldPos());
	m_pLightCam->Transform()->SetRelativeRot(DecomposeRotMat(Transform()->GetWorldRotation()));
	m_pLightCam->finaltick_module();


	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);
}

void CLight3D::render()
{
	if (nullptr == m_LightMtrl || nullptr == m_VolumeMesh)
		return;

	Transform()->UpdateData();

	// 쉐이더 func에서 CalcLight3D 시 LightIdx를 요구함
	m_LightMtrl->SetScalarParam(INT_0, &m_iLightIdx);
	// Light 여부 및 세기를 계산하는 데에 필요한 위치 및 노말값을 기록한 Tex를 보내줌
	m_LightMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	m_LightMtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));
	
	// 방향성 광원인 경우 그림자 처리를 위해서 광원카메라로 투영시킬 수 있게 View * Proj 행렬을 전달
	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.LightType)
	{
		Matrix matLightVP = m_pLightCam->Camera()->GetViewMat() * m_pLightCam->Camera()->GetProjMat();
		m_LightMtrl->SetScalarParam(MAT_0, &matLightVP);
		m_LightMtrl->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"DepthMapTex"));
	}

	m_LightMtrl->UpdateData();

	m_VolumeMesh->render(0);
}

void CLight3D::render_depthmap()
{
	m_pLightCam->Camera()->SortShadowObject();
	m_pLightCam->Camera()->render_depthmap();
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_LightInfo, sizeof(tLightInfo), 1, _File);

	SetLightType((LIGHT_TYPE)m_LightInfo.LightType);
	SetRadius(m_LightInfo.Radius);
	SetAngle(m_LightInfo.Angle);
	SetLightDiffuse(m_LightInfo.Color.vDiffuse);
	SetLightSpecular(m_LightInfo.Color.vSpecular);
	SetLightAmbient(m_LightInfo.Color.vAmbient);
}
