#include "pch.h"
#include "CPlyWpMagic_Hook.h"

CPlyWpMagic_Hook::CPlyWpMagic_Hook()
{
}

CPlyWpMagic_Hook::~CPlyWpMagic_Hook()
{
}

void CPlyWpMagic_Hook::Enter()
{
	GetOwner()->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::HOOK, false);
}

void CPlyWpMagic_Hook::tick()
{
}

void CPlyWpMagic_Hook::Exit()
{
	GetOwner()->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
}
