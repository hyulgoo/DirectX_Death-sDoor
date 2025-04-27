#include "pch.h"
#include "CBatAttack.h"
#include "CLevelSaveLoadInScript.h"

CBatAttack::CBatAttack()	
	: m_fTime(0.f)
	, m_bAttack(false)
{
}

CBatAttack::~CBatAttack()
{
}

void CBatAttack::Enter()
{
	GetOwner()->Animator3D()->Play(2, false);	
}

void CBatAttack::tick()
{
	m_fTime += DT;
	if(!m_bAttack && m_fTime >= 0.7f)
	{
		CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\BatAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.2f);
		m_bAttack = true;
	}
	else if (m_fTime >= 1.f && GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"Idle");
	}
}

void CBatAttack::Exit()
{
	m_fTime = 0.f;
	m_bAttack = false;
}
