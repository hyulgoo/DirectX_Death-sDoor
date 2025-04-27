#include "pch.h"
#include "CPlyWpLadder.h"

CPlyWpLadder::CPlyWpLadder()
{
}

CPlyWpLadder::~CPlyWpLadder()
{
}

void CPlyWpLadder::Enter()
{
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::LADDER_UP, true);
}

void CPlyWpLadder::tick()
{
}

void CPlyWpLadder::Exit()
{
}
