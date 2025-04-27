#include "pch.h"
#include "CStateScript.h"
#include "CGruntScript.h"
#include "GruntStates.h"
#include "CSoundScript.h"

#include <Engine/CDetourMgr.h>

CGruntScript::CGruntScript() :
	CMonsterScript((UINT)SCRIPT_TYPE::GRUNTSCRIPT)
	, m_fPlayerDistance(0.f)
	, m_fAttackRange(500.f)
	, m_iNailAttackCount(0)
	, m_bStarePlayer(false)
	, m_fPrevHP(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAttackRange, "AttackRange");
}

CGruntScript::CGruntScript(const CGruntScript& _Other) :
	CMonsterScript((UINT)SCRIPT_TYPE::GRUNTSCRIPT)
	, m_fPlayerDistance(_Other.m_fPlayerDistance)
	, m_fAttackRange(_Other.m_fAttackRange)
	, m_iNailAttackCount(0)
	, m_bStarePlayer(false)
	, m_fPrevHP(0.f)
{
}

CGruntScript::~CGruntScript()
{
}

void CGruntScript::CountNailAttack()
{
	if (m_iNailAttackCount > 1)
		m_iNailAttackCount = 0;

	else
		++m_iNailAttackCount;
}

void CGruntScript::begin()
{
	CMonsterScript::begin();
	
	// 동적 재질 생성.
	int iMtrlCount = MeshRender()->GetMtrlCount();

	for (int i = 0; i < iMtrlCount; ++i)
	{
		MeshRender()->GetDynamicMaterial(i);
	}

	// 상태 넣어주기.
	if (nullptr == m_pStateScript)
	{
		m_pStateScript = GetOwner()->GetScript<CStateScript>();
		m_pStateScript->AddState(L"BackStep", new CGruntBackStep);
		m_pStateScript->AddState(L"Death", new CGruntDeath);
		m_pStateScript->AddState(L"Idle", new CGruntIdle);
		m_pStateScript->AddState(L"JumpAttack", new CGruntJumpAttack);
		m_pStateScript->AddState(L"JumpAttackReady", new CGruntJumpAttackReady);
		m_pStateScript->AddState(L"LeftMove", new CGruntLeftMove);
		m_pStateScript->AddState(L"Move", new CGruntMove);
		m_pStateScript->AddState(L"NailAttack", new CGruntNailAttack);
		m_pStateScript->AddState(L"NailAttackReady", new CGruntNailAttackReady);
		m_pStateScript->AddState(L"RightMove", new CGruntRightMove);
		m_pStateScript->AddState(L"Run", new CGruntRun);
		m_pStateScript->AddState(L"Run2", new CGruntRun2);
		m_pStateScript->AddState(L"Trace", new CGruntTrace);
		m_pStateScript->AddState(L"Walk", new CGruntWalk);
		m_pStateScript->ChangeState(L"Idle");

		// 초기 스탯 설정.
		Stat NewStat;
		NewStat.Max_HP = 100;
		NewStat.HP = NewStat.Max_HP;
		NewStat.Attack = 50.f;
		NewStat.Attack_Speed = 1.f;
		NewStat.Speed = 100.f;
		m_pStateScript->SetStat(NewStat);

		// 이전 HP
		m_fPrevHP = NewStat.Max_HP;
	}
}

void CGruntScript::tick()
{
	CMonsterScript::tick();

	// HP가 0 이하면 사망.
	if (m_pStateScript && m_pStateScript->GetStat().HP <= 0)
	{
		if (m_pStateScript->FindState(L"Death") != m_pStateScript->GetCurState())
			m_pStateScript->ChangeState(L"Death");
	}
	
	m_vPlayerPos = GetPlayer()->Transform()->GetWorldPos();
	m_fPlayerDistance = GetDistance(m_vPlayerPos, GetOwner()->Transform()->GetWorldPos());
	
	m_vMonsterToPlayerDir = m_vPlayerPos - Transform()->GetWorldPos();
	m_vMonsterToPlayerDir.x /= m_fPlayerDistance;
	m_vMonsterToPlayerDir.y /= m_fPlayerDistance;
	m_vMonsterToPlayerDir.z /= m_fPlayerDistance;

	// 플레이어를 바라보는 경우.
	if (m_bStarePlayer)
	{
		float fDir = GetSmoothDir(GetOwner(), m_pPlayer);
		Vec3 CurDir = GetOwner()->Transform()->GetRelativeRot();
		GetOwner()->Transform()->SetRelativeRot(CurDir.x, fDir, 0.f);
	}

	float fCurHP = m_pStateScript->GetStat().HP;

	// 체력이 줄었다면.
	if (m_fPrevHP < fCurHP)
	{
		// Sound
		CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
		Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Grunt\\Grunt_Backstep1.ogg", 1, 0.1f);
		m_fPrevHP = fCurHP;
	}
}

void CGruntScript::BeginOverlap(CCollider3D* _Other)
{
}

void CGruntScript::OnOverlap(CCollider3D* _Other)
{
}

void CGruntScript::EndOverlap(CCollider3D* _Other)
{
}

void CGruntScript::SaveToLevelFile(FILE* _File)
{
}

void CGruntScript::LoadFromLevelFile(FILE* _File)
{
}
