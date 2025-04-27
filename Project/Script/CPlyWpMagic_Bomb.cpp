#include "pch.h"
#include "CPlyWpMagic_Bomb.h"

CPlyWpMagic_Bomb::CPlyWpMagic_Bomb()
{
}

CPlyWpMagic_Bomb::~CPlyWpMagic_Bomb()
{
}

void CPlyWpMagic_Bomb::Enter()
{
	GetOwner()->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::MAGIC_BOMB, false);
}

void CPlyWpMagic_Bomb::tick()
{
}

void CPlyWpMagic_Bomb::Exit()
{
	GetOwner()->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
}
