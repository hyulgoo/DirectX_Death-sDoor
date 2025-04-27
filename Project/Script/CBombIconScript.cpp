#include "pch.h"
#include "CBombIconScript.h"
#include "CPlayerScript.h"

CBombIconScript::CBombIconScript() :
	CIconScript(SCRIPT_TYPE::BOMBICONSCRIPT)
{
}

CBombIconScript::~CBombIconScript()
{
}

void CBombIconScript::begin()
{
}

void CBombIconScript::tick()
{
	
}

void CBombIconScript::MagicActive(bool _bActive)
{
	wstring wstrBombImageName;
	if (_bActive)
	{
		Transform()->SetRelativePos(Vec3(-587.f, 265.f, 0.f));
		Transform()->SetRelativeScale(Vec3(116.f, 100.f, 0.f));
		wstrBombImageName = L"texture\\HUD\\BombActive.png";
	}
	else
	{
		Transform()->SetRelativePos(Vec3(-595.f, 265.f, 0.f));
		Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
		wstrBombImageName = L"texture\\HUD\\BombDA.png";
	}
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(wstrBombImageName, wstrBombImageName, 0));
}
