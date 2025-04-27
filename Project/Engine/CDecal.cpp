#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"
#include "CResMgr.h"
#include "CDevice.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_DecalTex()
	, m_bShowDebug(false)
	, m_Light(0)
	, m_LightAlpha(1.f)
{
	SetName(L"Decal");
	// Decal은 항상 CubeMesh를 사용할 것이므로 생성자에서 설정
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DecalMtrl"), 0);
}

CDecal::~CDecal()
{
}

void CDecal::finaltick()
{
	if (m_bShowDebug)
	{
		const Matrix& matWorld = Transform()->GetWorldMat();
		DrawDebugCube(matWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f);
	}
}

void CDecal::render()
{
	Transform()->UpdateData();

	GetMaterial(0)->SetScalarParam(INT_0, &m_Light);
	GetMaterial(0)->SetScalarParam(FLOAT_0, &m_LightAlpha);
	GetMaterial(0)->SetTexParam(TEX_0, m_DecalTex);
	GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	GetMaterial(0)->UpdateData();

	GetMesh()->render(0);
}

void CDecal::render(UINT _iSubset)
{
	render();
}

void CDecal::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	fwrite(&m_bShowDebug, sizeof(bool), 1, _File);
	fwrite(&m_Light, sizeof(bool), 1, _File);
	fwrite(&m_LightAlpha, sizeof(float), 1, _File);
	SaveResRef(m_DecalTex.Get(), _File);
}

void CDecal::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_bShowDebug, sizeof(bool), 1, _File);
	fread(&m_Light, sizeof(bool), 1, _File);
	fread(&m_LightAlpha, sizeof(float), 1, _File);
	LoadResRef(m_DecalTex, _File);
}
