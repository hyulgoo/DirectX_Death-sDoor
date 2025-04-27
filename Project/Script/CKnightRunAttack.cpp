#include "pch.h"
#include "CKnightRunAttack.h"
#include "CLevelSaveLoadInScript.h"
#include "CKnightScript.h"
#include "CMonsterScript.h"

CKnightRunAttack::CKnightRunAttack()
	: m_fDelay(0.f)
	, m_bAttack(false)
{
}

CKnightRunAttack::~CKnightRunAttack()
{
}

void CKnightRunAttack::Enter()
{
	GetOwner()->GetScript<CKnightScript>()->SetDirtoPlayer();
	GetOwner()->Animator3D()->Play(2, false);

	float fDir = GetDir(GetOwner()->Transform()->GetWorldPos(), GetOwner()->GetScript<CMonsterScript>()->GetPlayer()->Transform()->GetWorldPos());
	Vec3 CurDir = GetOwner()->Transform()->GetRelativeRot();
	GetOwner()->Transform()->SetRelativeRot(CurDir.x, fDir, 0.f);
}

void CKnightRunAttack::tick()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_fDelay += DT;
	float fRatio = m_fDelay / GetOwner()->Animator3D()->GetCurClipTimeLength();
	if (fRatio > 0.8f && !m_bAttack)
	{
		CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\JumpAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.1f);
		m_bAttack = true;
	}
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"JumpReady");
	}
}

void CKnightRunAttack::Exit()
{
	m_fDelay = 0.f;
	m_bAttack = false;
	GetOwner()->Rigidbody()->ClearForce();
}
