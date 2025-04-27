#include "pch.h"
#include "CGhostScript.h"
#include "CTrace.h"
#include "CStateScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CPlyAttack.h"
#include "CGhostHit.h"
#include <Engine/CPhysXMgr.h>

CGhostScript::CGhostScript()		
	: CMonsterScript((UINT)SCRIPT_TYPE::GHOSTSCRIPT)
	, m_bRecognizeCheck(false)
	, m_bOnCollision(false)
	, m_bIsHit(false)
	, m_vPlayerPos{}
{
}

CGhostScript::CGhostScript(const CGhostScript& _Other)	
	: CMonsterScript((UINT)SCRIPT_TYPE::GHOSTSCRIPT)
	, m_bRecognizeCheck(_Other.m_bRecognizeCheck)
	, m_bOnCollision(false)
	, m_bIsHit(false)
	, m_vPlayerPos(_Other.m_vPlayerPos)
{
}

CGhostScript::~CGhostScript()
{
}

void CGhostScript::begin()
{
	// 동적 재질 생성.
	int iMtrlCount = MeshRender()->GetMtrlCount();

	for (int i = 0; i < iMtrlCount; ++i)
	{
		MeshRender()->GetDynamicMaterial(i);
	}

	if (nullptr == m_pStateScript)
	{
		m_pStateScript = GetOwner()->GetScript<CStateScript>();
		m_pStateScript->AddState(L"Trace", new CTrace);
		m_pStateScript->AddState(L"GhostHit", new CGhostHit);		
		m_pStateScript->ChangeState(L"Trace");
	}

	GetOwner()->Rigidbody()->SetVelocityLimit(300.f);

	// 초기 스탯 설정.
	Stat tInitStat;
	tInitStat.Attack = 1;
	tInitStat.Speed = 300;
	m_pStateScript->SetStat(tInitStat);

	SetLifeSpan(10.f);
}

void CGhostScript::tick()
{

}

Vec3 CGhostScript::GetDir(Vec3 _start, Vec3 _target)
{
	// 아래축을 기준으로 CurPos에서 TargetPos를 바라보는 angle 반환
	Vec3 vCurPos = _start;
	Vec2 vDefault = Vec2(0.f, -1.f);
	Vec3 vTargetPos = _target;
	Vec3 vDir = vTargetPos - vCurPos;
	vDir.y = 0;
	vDir.Normalize();

	return vDir;
}

void CGhostScript::BeginOverlap(CCollider3D* _Other)
{
	
	if (L"Slash_R" == _Other->GetOwner()->GetName() || L"Slash_L" == _Other->GetOwner()->GetName() && m_bIsHit == false)
	{
		//player가 바라 보는 방향으로 이동
		m_bIsHit = true;
		m_pStateScript->ChangeState(L"GhostHit");
	}
	if ((int)LAYER::PLAYER == _Other->GetOwner()->GetLayerIndex() && m_bIsHit ==false)
	{
		CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\GhostDead.prefab", (int)LAYER::DEFAULT, GetOwner()->Transform()->GetWorldPos(), 0.5f);
		GetOwner()->SetLifeSpan(0.f);
	}
	else if (_Other->GetOwner()->GetScript<CMonsterScript>() && m_bIsHit == true)
	{
		Stat tCurStat = _Other->GetOwner()->GetScript<CStateScript>()->GetStat();
		tCurStat.HP -= 50;
		_Other->GetOwner()->GetScript<CStateScript>()->SetStat(tCurStat);
		CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\GhostDead.prefab", (int)LAYER::DEFAULT, GetOwner()->Transform()->GetWorldPos(), 0.5f);
		GetOwner()->SetLifeSpan(0.f);
	}	
}

void CGhostScript::OnOverlap(CCollider3D* _Other)
{
}

void CGhostScript::EndOverlap(CCollider3D* _Other)
{
}

void CGhostScript::SaveToLevelFile(FILE* _File)
{
}

void CGhostScript::LoadFromLevelFile(FILE* _File)
{
}
