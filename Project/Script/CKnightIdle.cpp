#include "pch.h"
#include "CKnightIdle.h"
#include "CMonsterScript.h"

CKnightIdle::CKnightIdle()
{
}

CKnightIdle::~CKnightIdle()
{
}

void CKnightIdle::Enter()
{
	GetOwner()->Animator3D()->Play(0, false);
}

void CKnightIdle::tick()
{
	if (GetOwner()->GetScript<CMonsterScript>()->IsDetect())
		ChangeState(L"Trace");
}

void CKnightIdle::Exit()
{
}

void CKnightIdle::BeginOverlap(CCollider3D* _Other)
{
	if ((int)LAYER::PLAYER == _Other->GetOwner()->GetLayerIndex())
	{
		ChangeState(L"RunAttack");
	}
}
