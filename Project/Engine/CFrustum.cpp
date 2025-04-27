#include "pch.h"
#include "CFrustum.h"

#include "CCamera.h"

CFrustum::CFrustum(CCamera* _pOwner)
	: m_pOwner(_pOwner)
	, m_arrFace{}
	, m_bDebugShape(false)
{
	// 투영공간 좌표
	//     4 ------ 5
	//     |        |  Far
	//   / |        |
	//	/  7 ------ 6	
	// /      /
	// 0 -- 1     /
	// |    |    / Near
	// 3 -- 2
	m_arrProj[0] = Vec3(-1.f, 1.f, 0.f);
	m_arrProj[1] = Vec3(1.f, 1.f, 0.f);
	m_arrProj[2] = Vec3(1.f, -1.f, 0.f);
	m_arrProj[3] = Vec3(-1.f, -1.f, 0.f);

	m_arrProj[4] = Vec3(-1.f, 1.f, 1.f);
	m_arrProj[5] = Vec3(1.f, 1.f, 1.f);
	m_arrProj[6] = Vec3(1.f, -1.f, 1.f);
	m_arrProj[7] = Vec3(-1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::finaltick()
{
	Vec3 vWorldPos[8] = {};

	// MDC 기준으로 잡아놓은 평면좌표를 투영, 뷰행렬의 역행렬을 곱해 월드좌표계로 변환시킬 것
	Matrix matInv = m_pOwner->GetProjMatInv() * m_pOwner->GetViewMatInv();

	for (int i = 0; i < 8; ++i)
	{
		vWorldPos[i] = XMVector3TransformCoord(m_arrProj[i], matInv);
	}

	// 3개의 점을 이용해 해당 점을 이은 평면과 수직하는 법선벡터를 만듦
	m_arrFace[FT_NEAR]  = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
	m_arrFace[FT_FAR]   = XMPlaneFromPoints(vWorldPos[6], vWorldPos[5], vWorldPos[4]);
	m_arrFace[FT_LEFT]  = XMPlaneFromPoints(vWorldPos[4], vWorldPos[0], vWorldPos[7]);
	m_arrFace[FT_RIGHT] = XMPlaneFromPoints(vWorldPos[1], vWorldPos[5], vWorldPos[6]);
	m_arrFace[FT_TOP]	= XMPlaneFromPoints(vWorldPos[4], vWorldPos[5], vWorldPos[1]);
	m_arrFace[FT_BOT]	= XMPlaneFromPoints(vWorldPos[2], vWorldPos[6], vWorldPos[7]); 

	// 카메라 시야 Debug Render
	if (m_bDebugShape)
	{
		DrawDebugFrustum(matInv, Vec4(0.f, 1.f, 0.f, 1.f));
	}
}

bool CFrustum::FrustumCheckByPoint(Vec3 _vWorldPos)
{	
	for (int i = 0; i < FT_END; ++i)
	{
		// 임의의 점과 해당 평면의 법선벡터와 내적
		Vec3 vNormal = m_arrFace[i];
		vNormal.Normalize();
		float fDot = vNormal.Dot(_vWorldPos);

		// 내적한 값과 원점에서 해당 평면까지의 최단거리(D) 와 비교
		// a, b, c가 법선벡터의 xyz라고 할 때 
		// ax + by + cz + d = 0 이 해당 평면에 존재할 때 성립되는 평면의 방정식
		// xyz는 WorldPos이므로 ax+ by + cz + d가 0보다 크다면 해당 평면보다 밖에 있다는 것
		// 현재 모든 법선벡터를 시야범위 바깥방향으로 했으므로 false 반환
		if (fDot + m_arrFace[i].w > 0)
			return false;
	}

	return true;
}

bool CFrustum::FrustumCheckBySphere(Vec3 _vWorldPos, float _fRadius)
{
	for (int i = 0; i < FT_END; ++i)
	{
		// 임의의 점과 해당 평면의 법선벡터와 내적
		Vec3 vNormal = m_arrFace[i];
		vNormal.Normalize();
		float fDot = vNormal.Dot(_vWorldPos);

		// 내적한 값과 원점에서 해당 평면까지의 최단거리(D) 와 비교
		// 반지름보다 더 바깥으로 벗어났어야 화면에 걸치는 부분이 없는 것
		if (fDot + m_arrFace[i].w > _fRadius)
			return false;
	}

	return true;
}
