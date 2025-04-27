#include "pch.h"
#include "CCrowBossSliding.h"
#include "CCrowBossScript.h"
#include "CBossChainScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CSoundScript.h"

CCrowBossSliding::CCrowBossSliding()
	: m_qStartPos{}
	, m_qTargetPos{}
	, m_vDirtoTarget{}
	, m_iSliderCount(0)
	, m_tChainPatern()
{
}

CCrowBossSliding::~CCrowBossSliding()
{
}

void CCrowBossSliding::Enter()
{
	GetOwner()->Animator3D()->Play(3, false);

	// 첫 방향을 설정
	Vec3 vStartPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vTargetPos = m_qTargetPos.front();
	if (!m_qStartPos.empty())
	{
		vStartPos = m_qStartPos.front();
		GetOwner()->Rigidbody()->SetRigidPos(vStartPos);
		m_qStartPos.pop();
	}

	m_vDirtoTarget = vTargetPos - vStartPos;
	m_vDirtoTarget.Normalize();

	float YRot = GetDir(vStartPos, vTargetPos);
	GetOwner()->Transform()->SetRelativeRot(XM_PI * 1.5f, YRot, 0.f);

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\CrowBoss\\OldCrowChainDash1.ogg", 1, 0.1f);
}

void CCrowBossSliding::tick()
{
	// 공격 충돌체 프리펩
	CGameObject* MonsterAtack = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\MonsterAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.f);

	MonsterAtack->Collider3D()->SetOffsetPos(GetOwner()->Collider3D()->GetOffsetPos());
	MonsterAtack->Collider3D()->SetOffsetScale(GetOwner()->Collider3D()->GetOffsetScale());

	SlidingToTargetPos();

	Vec3 vCurPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vDiff = m_qTargetPos.front() - vCurPos;
	vDiff.y = 0.f;
	float fDiff = vDiff.Length();

	// 추가된 목적지를 도착할 때마다 목적지를 삭제함
	if (fDiff < 100.f)
	{
		ArriveToTarget();
	}
	if(!m_qTargetPos.empty())
	{
		float fDir = GetDir(GetOwner()->Transform()->GetWorldPos(), m_qTargetPos.front());
		Vec3 CurDir = GetOwner()->Transform()->GetRelativeRot();
		GetOwner()->Transform()->SetRelativeRot(CurDir.x, fDir, 0.f);
	}
}

void CCrowBossSliding::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_iSliderCount = 0;
	m_vDirtoTarget = {};
	m_tChainPatern = {};
	queue<Vec3> empty = {};
	m_qStartPos.swap(empty);
	m_qTargetPos.swap(empty);

	for (size_t i = 0; i < m_vecHook.size(); ++i)
	{
		m_vecHook[i]->GetScript<CBossChainScript>()->Active(false, false, 0.f);
		m_vecHook[i]->GetScript<CBossChainScript>()->Clear();
	}
}

void CCrowBossSliding::AddTargetPos(Vec3 _vTargetPos, Vec3 _vStartPos)
{
	if (CHAINPATERN::ONE != m_tChainPatern)
	{
		m_qStartPos.push(_vStartPos);
	}

	m_qTargetPos.push(_vTargetPos);

	++m_iSliderCount;
}

void CCrowBossSliding::SlidingToTargetPos()
{
	Vec3 vVelocity = m_vDirtoTarget * 1200.f;
	GetOwner()->Rigidbody()->SetVelocity(vVelocity);
	GetOwner()->Transform()->SetRelativeRot(XM_PI * 1.5f, acos(Vec2(0.f, -1.f).Dot(Vec2(-m_vDirtoTarget.x, m_vDirtoTarget.z))), 0.f);

	int iSlidingCount = 0;
	switch (m_tChainPatern)
	{
	case CHAINPATERN::ONE:
		iSlidingCount = 1;
		break;
	case CHAINPATERN::CROSS:
		iSlidingCount = 2;
		break;
	case CHAINPATERN::SPREAD:
		iSlidingCount = 5;
		break;
	}

	Vec3 CrowMoveDist = GetOwner()->Transform()->GetRelativePos() - GetOwner()->Transform()->GetPrevPos();
	
	// 순간이동이 아닌 슬라이딩 시 타고 있는 사슬을 같이 밀어냄
	if(CrowMoveDist.Length() < 100.f)
	{
		Vec3 vCurHookPos = m_vecHook[iSlidingCount - m_iSliderCount]->Transform()->GetRelativePos();
		m_vecHook[iSlidingCount - m_iSliderCount]->Transform()->SetRelativePos(vCurHookPos + CrowMoveDist);
		vector<CGameObject*> vecChain = m_vecHook[iSlidingCount - m_iSliderCount]->GetScript<CBossChainScript>()->GetChain();
		for (size_t i = 0; i < vecChain.size(); ++i)
		{
			Vec3 vCurChainPos = vecChain[i]->Transform()->GetRelativePos();
			vecChain[i]->Transform()->SetRelativePos(vCurChainPos + CrowMoveDist);
		}
	}
}

void CCrowBossSliding::ArriveToTarget()
{
	--m_iSliderCount;
	m_qTargetPos.pop(); 		
	// TargetPos가 없다면 다른 패턴으로 전환
	// 아니라면 도착한 목적지에 있던 사슬을 비활성화
	if (!m_qTargetPos.empty())
	{
		int iSlidingCount = 0;
		switch (m_tChainPatern)
		{
		case CHAINPATERN::ONE:
			iSlidingCount = 1;
			break;
		case CHAINPATERN::CROSS:
			iSlidingCount = 2;
			break;
		case CHAINPATERN::SPREAD:
			iSlidingCount = 5;
			break;
		}
		Vec3 vStartPos = GetOwner()->Transform()->GetRelativePos();
		// 여러 개의 갈고리를 던지는 패턴을 경우 StartPos로 이동과 함께 회전
		if(CHAINPATERN::ONE != m_tChainPatern)
		{
			vStartPos = m_qStartPos.front();
			GetOwner()->Rigidbody()->SetRigidPos(vStartPos);
			m_qStartPos.pop();
		}

		// Sound
		CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
		Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\CrowBoss\\OldCrowChainDash1.ogg", 1, 0.1f);

		Vec3 vTargetPos = m_qTargetPos.front();
		m_vDirtoTarget = vTargetPos - vStartPos;
		m_vDirtoTarget.y = 0.f;
		m_vDirtoTarget.Normalize();
	}
	else
	{
		if (CHAINPATERN::ONE == m_tChainPatern)
			ChangeState(L"SlidingReady");
		else
			ChangeState(L"Jump");
	}
}
