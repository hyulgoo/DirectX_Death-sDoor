#include "pch.h"
#include "CBatRecognize.h"

CBatRecognize::CBatRecognize()
{
}

CBatRecognize::~CBatRecognize()
{
}

void CBatRecognize::Enter()
{
	GetOwner()->Animator3D()->Play(1, false);
}

void CBatRecognize::tick()
{
	if(GetOwner()->Animator3D()->IsFinish())
		ChangeState(L"Trace");
}

void CBatRecognize::Exit()
{
}
