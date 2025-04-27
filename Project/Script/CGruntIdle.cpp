#include "pch.h"
#include "CGruntIdle.h"
#include "CGruntScript.h"

void CGruntIdle::Enter()
{
	GetOwner()->Animator3D()->Play(0, true);
}

void CGruntIdle::tick()
{
	CGameObject* pPlayer = GetOwner()->GetScript<CGruntScript>()->GetPlayer();

	// Detect 상태라면 Notify 상태로 전환.
	if (GetOwner()->GetScript<CGruntScript>()->IsDetect())
	{
		ChangeState(L"Run2");
	}
}

void CGruntIdle::Exit()
{
}

CGruntIdle::CGruntIdle()
{
}

CGruntIdle::~CGruntIdle()
{
}
