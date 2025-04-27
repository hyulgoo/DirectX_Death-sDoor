#include "pch.h"
#include "CKnightJumpReady.h"


CKnightJumpReady::CKnightJumpReady()
{
}

CKnightJumpReady::~CKnightJumpReady()
{
}

void CKnightJumpReady::Enter()
{
	GetOwner()->Animator3D()->Play(5, false);
}
void CKnightJumpReady::tick()
{
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"JumpAttack1");
	}
}


void CKnightJumpReady::Exit()
{
}