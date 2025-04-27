#include "pch.h"
#include "CGruntNailAttackReady.h"
#include "CGruntScript.h"

void CGruntNailAttackReady::Enter()
{
	GetOwner()->Animator3D()->Play(2, false);
}

void CGruntNailAttackReady::tick()
{
	// 애니메이션이 끝나면 NailAttack로 다시 변경.
	if (GetOwner()->Animator3D()->IsFinish())
		ChangeState(L"NailAttack");
}

void CGruntNailAttackReady::Exit()
{
}

CGruntNailAttackReady::CGruntNailAttackReady()
{
}

CGruntNailAttackReady::~CGruntNailAttackReady()
{
}
