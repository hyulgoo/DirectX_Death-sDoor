#include "pch.h"
#include "CPlyWpMagic_Arrow.h"

CPlyWpMagic_Arrow::CPlyWpMagic_Arrow()
{
}

CPlyWpMagic_Arrow::~CPlyWpMagic_Arrow()
{
}

void CPlyWpMagic_Arrow::Enter()
{
	GetOwner()->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::ARROW, false);
}

void CPlyWpMagic_Arrow::tick()
{
}

void CPlyWpMagic_Arrow::Exit()
{
	GetOwner()->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
}
