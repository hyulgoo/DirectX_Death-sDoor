#include "pch.h"
#include "CPlyWpRun.h"

CPlyWpRun::CPlyWpRun()
{
}

CPlyWpRun::~CPlyWpRun()
{
}

void CPlyWpRun::Enter()
{
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::RUN, true);
}

void CPlyWpRun::tick()
{
}

void CPlyWpRun::Exit()
{
}
