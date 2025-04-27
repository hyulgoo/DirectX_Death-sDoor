#include "pch.h"
#include "CLurkerScript.h"
#include "CStateScript.h"
#include "LurkerStates.h"
#include "CSoundScript.h"

#include <Engine/CDetourMgr.h>

CLurkerScript::CLurkerScript() :
	CMonsterScript((UINT)SCRIPT_TYPE::LURKERSCRIPT)
	, m_fPlayerDistance(0.f)
	, m_fBackStepRange(250.f)
	, m_fAttackRange(500.f)
	, m_bStarePlayer(false)
	, m_fPrevHP(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fBackStepRange, "BackStepRange");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAttackRange, "AttackRange");
}

CLurkerScript::CLurkerScript(const CLurkerScript& _Other) :
	CMonsterScript((UINT)SCRIPT_TYPE::LURKERSCRIPT)
	, m_fPlayerDistance(_Other.m_fPlayerDistance)
	, m_fBackStepRange(_Other.m_fBackStepRange)
	, m_fAttackRange(_Other.m_fAttackRange)
	, m_bStarePlayer(false)
	, m_fPrevHP(0.f)
{
}

CLurkerScript::~CLurkerScript()
{
}

void CLurkerScript::begin()
{
	CMonsterScript::begin();

	// 상태 넣어주기.
	if (nullptr == m_pStateScript)
	{
		m_pStateScript = GetOwner()->GetScript<CStateScript>();
		m_pStateScript->AddState(L"Attack", new CLurkerAttack);
		m_pStateScript->AddState(L"BackMove", new CLurkerBackMove);
		m_pStateScript->AddState(L"BackStep", new CLurkerBackStep);
		m_pStateScript->AddState(L"Death", new CLurkerDeath);
		m_pStateScript->AddState(L"FrontMove", new CLurkerFrontMove);
		m_pStateScript->AddState(L"Idle", new CLurkerIdle);
		m_pStateScript->AddState(L"LeftMove", new CLurkerLeftMove);
		m_pStateScript->AddState(L"Notify", new CLurkerNotify);
		m_pStateScript->AddState(L"RightMove", new CLurkerRightMove);
		m_pStateScript->AddState(L"Trace", new CLurkerTrace);
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

void CLurkerScript::tick()
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
		Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Lurker\\LurkerTakeDamage1.ogg", 1, 0.1f);
		m_fPrevHP = fCurHP;
	}
}

void CLurkerScript::BeginOverlap(CCollider3D* _Other)
{
}

void CLurkerScript::OnOverlap(CCollider3D* _Other)
{
}

void CLurkerScript::EndOverlap(CCollider3D* _Other)
{
}

void CLurkerScript::SaveToLevelFile(FILE* _File)
{
}

void CLurkerScript::LoadFromLevelFile(FILE* _File)
{
}
