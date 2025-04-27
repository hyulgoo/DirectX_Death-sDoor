#include "pch.h"
#include "CGrimKnightBackStep2.h"

CGrimKnightBackStep2::CGrimKnightBackStep2()
{
}

CGrimKnightBackStep2::~CGrimKnightBackStep2()
{
}

void CGrimKnightBackStep2::Enter()
{
	GetOwner()->Animator3D()->Play(10, false);
}

void CGrimKnightBackStep2::tick()
{
	if (GetOwner()->Animator3D()->IsFinish())
	{
		ChangeState(L"LongDistance");
	}
	GetOwner()->Rigidbody()->ClearForce();
}

void CGrimKnightBackStep2::Exit()
{
}
