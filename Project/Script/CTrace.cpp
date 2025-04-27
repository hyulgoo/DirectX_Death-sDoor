#include "pch.h"
#include "CTrace.h"
#include "CMonsterScript.h"
#include "CSoundScript.h"

#include <Engine/CDetourMgr.h>

CTrace::CTrace()
	: m_fLastRenewal(0.f)
	, m_fRenewal_Trace(0.3f)
	, m_vActualPath{}
	, m_iActualPathCount(0)
	, m_iCurrentPathIndex(0)
{
}

CTrace::~CTrace()
{
}

void CTrace::Enter()
{
	if (L"Bat" == GetOwner()->GetName())
	{
		GetOwner()->Animator3D()->Play(3, true);
	}
	else if (L"GrimKnight" == GetOwner()->GetName())
	{
		GetOwner()->Animator3D()->Play(6, true);

		CSoundScript* pSoundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
		Ptr<CSound> pSound = pSoundscript->AddSound(L"Sound\\Monster\\Grim\\GrimaceStep1.ogg", 1, 0.1f);
	}
}

void CTrace::tick()
{
	float fSpeed = GetOwnerScript()->GetStat().Speed;

	m_fLastRenewal += DT;
	if (m_fLastRenewal >= m_fRenewal_Trace)
	{
		for (int i = 0; i < 256; ++i)
		{
			m_vActualPath[i] = Vec3(0.f, 0.f, 0.f);
		}
		m_iCurrentPathIndex = 0;
		m_iActualPathCount = 0;
		m_fLastRenewal -= m_fRenewal_Trace;
		Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
		memcpy(m_vActualPath, CDetourMgr::GetInst()->GetPathtoTarget(CurPos, &m_iActualPathCount), sizeof(Vec3) * 256);
	}

	if (m_iCurrentPathIndex < m_iActualPathCount)
	{
		// 다음 노드(메시) 위치
		Vec3 targetPos = m_vActualPath[m_iCurrentPathIndex];
		targetPos.z *= -1.f;
		if (targetPos.x == 0 && targetPos.y == 0 && targetPos.z == 0)
		{
			return;
		}
		// 현재 오브젝트 위치		
		Vec3 vCurrentPos = GetOwner()->Transform()->GetWorldPos();

		// 이동할 방향 벡터 계산 및 정규화
		Vec3 vDirection = targetPos - vCurrentPos;
		vDirection.Normalize();

		// 새로운 위치 계산
		Vec3 newPos = vCurrentPos + vDirection * fSpeed * DT;
		vDirection.y = 0.f;

		GetOwner()->Rigidbody()->SetVelocity(vDirection * fSpeed);
		GetOwner()->Transform()->CalcDir();

		// 만약 타겟 위치에 도달했다면, 다음 경로 인덱스.
		float distanceToTarget = (targetPos - vCurrentPos).Length();
		if (distanceToTarget < 50.f)
		{
			++m_iCurrentPathIndex;
		}
	}
	//sound
	if (GetOwner()->GetName() == L"GrimKnight" || GetOwner()->GetName() == L"Bat")
	{
		if ((GetOwner()->GetScript<CMonsterScript>()->GetPlayer()->Transform()->GetWorldPos()
			- GetOwner()->Transform()->GetWorldPos()).Length() < 100.f)
			ChangeState(L"Attack");
	}
}

void CTrace::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
}
