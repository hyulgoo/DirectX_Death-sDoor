#include "pch.h"
#include "CPlyWpDead.h"

CPlyWpDead::CPlyWpDead()
{
}

CPlyWpDead::~CPlyWpDead()
{
}

void CPlyWpDead::Enter()
{
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::HIT_IDLE, false);
}

void CPlyWpDead::tick()
{
}

void CPlyWpDead::Exit()
{
}
