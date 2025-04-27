#include "pch.h"
#include "CGruntJumpAttackReady.h"
#include "CGruntScript.h"

void CGruntJumpAttackReady::Enter()
{
	GetOwner()->Animator3D()->Play(11, false);
}

void CGruntJumpAttackReady::tick()
{
	// 애니메이션이 끝나면 NailAttack로 다시 변경.
	if (GetOwner()->Animator3D()->IsFinish())
		ChangeState(L"JumpAttack");
}

void CGruntJumpAttackReady::Exit()
{
}

CGruntJumpAttackReady::CGruntJumpAttackReady()
{
}

CGruntJumpAttackReady::~CGruntJumpAttackReady()
{
}
