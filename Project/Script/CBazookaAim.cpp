#include "pch.h"
#include "CBazookaAim.h"
#include "CBazookaScript.h"

void CBazookaAim::Enter()
{
	GetOwner()->Animator3D()->Play(0, false);
}

void CBazookaAim::tick()
{
	// 조준 중에 플레이어가 너무 가깝다면 도망가거나, 근접 공격을 해야 한다. 
	Vec3 PlayerPos = GetOwner()->GetScript<CMonsterScript>()->GetPlayer()->Transform()->GetWorldPos();

	float fDistance = GetOwner()->GetScript<CBazookaScript>()->GetPlayerDistance();

	if (fDistance < GetOwner()->GetScript<CBazookaScript>()->GetRunAwayRange())
	{
		ChangeState(L"Move");
	}
	
	else if(GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"LongDistance");
	}
}

void CBazookaAim::Exit()
{
}

CBazookaAim::CBazookaAim()
{
}

CBazookaAim::~CBazookaAim()
{
}
