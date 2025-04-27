#include "pch.h"
#include "CFireIconScript.h"
#include "CPlayerScript.h"

CFireIconScript::CFireIconScript() :
	CIconScript(SCRIPT_TYPE::FIREICONSCRIPT)
{
}

CFireIconScript::~CFireIconScript()
{
}

void CFireIconScript::begin()
{
}

void CFireIconScript::tick()
{	
}

void CFireIconScript::MagicActive(bool _bActive)
{
	wstring wstrHookImageName;
	if (_bActive)
	{
		Transform()->SetRelativePos(Vec3(-709.f, 265.f, 0.f));
		Transform()->SetRelativeScale(Vec3(116, 100.f, 0.f));

		wstrHookImageName = L"texture\\HUD\\FireActive.png";
	}
	else
	{
		Transform()->SetRelativePos(Vec3(-709.f, 265.f, 0.f));
		Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
		wstrHookImageName = L"texture\\HUD\\FireDA.png";
	}

	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(wstrHookImageName, wstrHookImageName, 0));
}
