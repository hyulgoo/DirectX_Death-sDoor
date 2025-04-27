#include "pch.h"
#include "CBazzokaMove.h"
#include "CBazookaScript.h"
#include <Engine/CDetourMgr.h>

void CBazzokaMove::Enter()
{
	m_fMoveTime = 0.f;
	GetOwner()->Animator3D()->Play(4, true);
}

void CBazzokaMove::tick()
{
	// Player 응시
	GetOwner()->GetScript<CBazookaScript>()->SetStarePlayer(true);

	Vec3 PlayerPos = GetOwner()->GetScript<CBazookaScript>()->GetPlayerPos();

	float fDistance = GetOwner()->GetScript<CBazookaScript>()->GetPlayerDistance();

	// 근접 공격 범위면 Melee
	if (fDistance < GetOwner()->GetScript<CBazookaScript>()->GetMeleeRange())
	{
		ChangeState(L"Melee");
	}

	// 도망. 여기서는 달리는 곳을 바라보도록 회전시켜줘야 한다. 
	else if (fDistance <= GetOwner()->GetScript<CBazookaScript>()->GetRunAwayRange() || m_fMoveTime <= 0.5f)
	{
		m_fMoveTime += DT;
		
		GetOwner()->GetScript<CBazookaScript>()->SetStarePlayer(false);
		
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

			// 현재 위치에서 플레이어와의 차이만큼 더해준 곳으로 이동시키자.
			Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
			Vec3 TargetPos = CurPos + CurPos - PlayerPos;
			memcpy(m_vActualPath, CDetourMgr::GetInst()->GetPathtoTarget(CurPos, TargetPos, &m_iActualPathCount), sizeof(Vec3) * 256);
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
			Vec3 currentPos = GetOwner()->Transform()->GetWorldPos();

			// 이동할 방향 벡터 계산 및 정규화
			Vec3 direction = targetPos - currentPos;
			direction.Normalize();

			// 새로운 위치 계산
			Vec3 newPos = currentPos + direction * fSpeed * DT;
			direction.y = 0.f;

			GetOwner()->Rigidbody()->SetVelocity(direction * fSpeed);

			// 만약 타겟 위치에 도달했다면, 다음 경로 인덱스.
			float distanceToTarget = (targetPos - currentPos).Length();
			if (distanceToTarget < 50.f)
			{
				++m_iCurrentPathIndex;
			}

			// 가는 곳을 바라보도록 회전 시키기.
			Vec3 CurDir = GetOwner()->Transform()->GetRelativeRot();
			float fDir = GetSmoothDir(currentPos, targetPos, CurDir);
			GetOwner()->Transform()->SetRelativeRot(CurDir.x, fDir, 0.f);
		}
	}

	// 공격범위
	else if (fDistance < GetOwner()->GetScript<CBazookaScript>()->GetAttackRange() && fDistance > GetOwner()->GetScript<CBazookaScript>()->GetRunAwayRange())
	{
		ChangeState(L"Aim");
	}

	// 범위 이상이면 Trace.
	else
	{
		ChangeState(L"Trace");
	}
}

void CBazzokaMove::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
}

CBazzokaMove::CBazzokaMove()
	: m_fLastRenewal(0.f)
	, m_fRenewal_Trace(0.2f)
	, m_vActualPath{}
	, m_iActualPathCount(0)
	, m_iCurrentPathIndex(0)
	, m_fMoveTime(0.f)
{
}

CBazzokaMove::~CBazzokaMove()
{
}
