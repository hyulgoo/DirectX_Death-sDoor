#include "pch.h"
#include "CGrimKnightIdle.h"
#include "CMonsterScript.h"

CGrimKnightIdle::CGrimKnightIdle()
{
}

CGrimKnightIdle::~CGrimKnightIdle()
{
}

void CGrimKnightIdle::Enter()
{
	GetOwner()->Animator3D()->Play(0, true);
}

void CGrimKnightIdle::tick()
{
	if (GetOwner()->GetScript<CMonsterScript>()->IsDetect())
		ChangeState(L"LongDistance");
}

void CGrimKnightIdle::Exit()
{
}
