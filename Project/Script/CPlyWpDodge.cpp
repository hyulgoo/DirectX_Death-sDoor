#include "pch.h"
#include "CPlyWpDodge.h"

CPlyWpDodge::CPlyWpDodge()
{
}

CPlyWpDodge::~CPlyWpDodge()
{
}

void CPlyWpDodge::Enter()
{
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::DODGE, false);
}

void CPlyWpDodge::tick()
{
}

void CPlyWpDodge::Exit()
{
}

