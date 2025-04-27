#include "pch.h"
#include "CGrimKnightBackStep.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CGrimKnightBackStep::CGrimKnightBackStep()
{
}

CGrimKnightBackStep::~CGrimKnightBackStep()
{
}

void CGrimKnightBackStep::Enter()
{
	//Stat status = GetOwnerScript()->GetStat();
	GetOwner()->Animator3D()->Play(9, false);

	//플레이어 반대 방향으로 이동
	Vec3 vPlayerPos = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->Transform()->GetWorldPos();
	Vec3 vDir = (GetOwner()->Transform()->GetWorldPos() - vPlayerPos).Normalize();
	GetOwner()->Rigidbody()->AddVelocity(vDir * 150.f);
}

void CGrimKnightBackStep::tick()
{
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"BackStep2");
	}
}

void CGrimKnightBackStep::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
}
