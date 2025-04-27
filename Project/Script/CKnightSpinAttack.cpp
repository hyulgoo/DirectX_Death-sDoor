#include "pch.h"
#include "CKnightSpinAttack.h"
#include "CLevelSaveLoadInScript.h"

CKnightSpinAttack::CKnightSpinAttack()
{
}

CKnightSpinAttack::~CKnightSpinAttack()
{
}

void CKnightSpinAttack::Enter()
{
	GetOwner()->Animator3D()->Play(12, false);
	CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\SpinAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.2f);
}

void CKnightSpinAttack::tick()
{
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"Trace");
	}	
}

void CKnightSpinAttack::Exit()
{
}
