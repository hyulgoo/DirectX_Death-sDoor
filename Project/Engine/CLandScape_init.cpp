#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"



void CLandScape::init()
{
	SetFace(1, 1);

	//SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"), 0);

	CreateComputeShader();

	CreateTexture();

	// Raycast 결과를 받는 버퍼
	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);

	// 타일 텍스쳐(Color, Normal 혼합으로 총 6장을 받음)
	m_pTileArrTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_ARRR.dds");
	m_pTileArrTex->GenerateMip(8);
}

void CLandScape::CreateMesh()
{
	if (GetMesh().Get())
	{
		SetMesh(nullptr);
	}

	Vtx v;
	vector<Vtx> vecVtx;

	for (UINT i = 0; i <= m_iFaceZ; ++i)
	{
		for (UINT j = 0; j <= m_iFaceX; ++j)
		{
			v.vPos = Vec3((float)j, 0.f, (float)i);
			v.vUV = Vec2((float)j, (float)m_iFaceZ - i);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	vector<UINT> vecIdx;

	for (UINT i = 0; i < m_iFaceZ; ++i)
	{
		for (UINT j = 0; j < m_iFaceX; ++j)
		{
			// 0
			// | \
			// 2--1  
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + j);
			vecIdx.push_back((m_iFaceX + 1) * i + (j + 1));
			vecIdx.push_back((m_iFaceX + 1) * i + j);

			// 0--1
			//  \ |
			//    2
			vecIdx.push_back((m_iFaceX + 1) *  i + (j + 1));
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + j);
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j + 1));
		}
	}

	Ptr<CMesh> pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	SetMesh(pMesh);

	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"), 0);
}

void CLandScape::CreateComputeShader()
{
	// 높이 수정 Compute Shader
	m_pCSHeightMap = (CHeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HeightMapShader").Get();
	if (nullptr == m_pCSHeightMap)
	{
		m_pCSHeightMap = new CHeightMapShader(32, 32, 1);
		m_pCSHeightMap->CreateComputeShader(L"shader\\heightmap.fx", "CS_HeightMap");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"HeightMapShader", m_pCSHeightMap.Get());
	}

	// 지형 피킹 Compute Shader
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastShader").Get();
	if (nullptr == m_pCSRaycast)
	{
		m_pCSRaycast = new CRaycastShader(32, 32, 1);
		m_pCSRaycast->CreateComputeShader(L"shader\\raycast.fx", "CS_Raycast");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"RaycastShader", m_pCSRaycast.Get());
	}

	// 가중치 수정 Compute Shader
	m_pCSWeightMap = (CWeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"WeightMapShader").Get();
	if (nullptr == m_pCSWeightMap)
	{
		m_pCSWeightMap = new CWeightMapShader(32, 32, 1);
		m_pCSWeightMap->CreateComputeShader(L"shader\\weightmap.fx", "CS_WeightMap");
		CResMgr::GetInst()->AddRes<CComputeShader>(L"WeightMapShader", m_pCSWeightMap.Get());
	}
}

void CLandScape::CreateTexture()
{
	m_pHeightMap = CResMgr::GetInst()->FindRes<CTexture>(L"HeightMap");
	if(nullptr == m_pHeightMap)
	{
		// 높이맵 텍스쳐
		m_pHeightMap = CResMgr::GetInst()->CreateTexture(L"HeightMap"
			, 2048, 2048
			, DXGI_FORMAT_R32_FLOAT
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
			, D3D11_USAGE_DEFAULT);
	}

	m_pBrushTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\brush\\Brush_01.png");

	// 가중치 버퍼
	m_iWeightWidth = 1024;
	m_iWeightHeight = 1024;

	if(nullptr == m_pWeightMapBuffer)
	{
		m_pWeightMapBuffer = new CStructuredBuffer;
		m_pWeightMapBuffer->Create(sizeof(tWeight_4), m_iWeightWidth * m_iWeightHeight, SB_TYPE::READ_WRITE, false);
	}
}
