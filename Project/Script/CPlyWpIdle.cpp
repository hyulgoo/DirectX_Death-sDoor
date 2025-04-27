#include "pch.h"
#include "CPlyWpIdle.h"

CPlyWpIdle::CPlyWpIdle()
{
}

CPlyWpIdle::~CPlyWpIdle()
{
}

void CPlyWpIdle::Enter()
{
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::IDLE, true);
}

void CPlyWpIdle::tick()
{
}

void CPlyWpIdle::Exit()
{
}
