#include "pch.h"
#include "CPlyWpMagic_Fire.h"

CPlyWpMagic_Fire::CPlyWpMagic_Fire()
{
}

CPlyWpMagic_Fire::~CPlyWpMagic_Fire()
{
}

void CPlyWpMagic_Fire::Enter()
{
	GetOwner()->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::MAGIC_FIRE, false);
}

void CPlyWpMagic_Fire::tick()
{
}

void CPlyWpMagic_Fire::Exit()
{
	GetOwner()->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
}

