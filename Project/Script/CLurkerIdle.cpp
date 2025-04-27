#include "pch.h"
#include "CLurkerIdle.h"
#include "CLurkerScript.h"

void CLurkerIdle::Enter()
{
	GetOwner()->Animator3D()->Play(0, true);
}

void CLurkerIdle::tick()
{
	CGameObject* pPlayer = GetOwner()->GetScript<CLurkerScript>()->GetPlayer();

	// Detect 상태라면 Notify 상태로 전환.
	if (GetOwner()->GetScript<CLurkerScript>()->IsDetect())
	{
		ChangeState(L"Notify");
	}
}

void CLurkerIdle::Exit()
{
}

CLurkerIdle::CLurkerIdle()
{
}

CLurkerIdle::~CLurkerIdle()
{
}
