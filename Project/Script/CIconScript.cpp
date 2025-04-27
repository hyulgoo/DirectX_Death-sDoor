#include "pch.h"
#include "CIconScript.h"

CIconScript::CIconScript(UINT SCRIPT_TYPE) :
	CScript(SCRIPT_TYPE)
{
}

CIconScript::~CIconScript()
{
}

void CIconScript::Disable()
{
	Transform()->SetRelativeScale(Vec3(0.f));
}
