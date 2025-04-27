#include "pch.h"
#include "CArrowIconScript.h"
#include "CPlayerScript.h"
#include "CLevelSaveLoadinScript.h"

CArrowIconScript::CArrowIconScript() :
	CIconScript(SCRIPT_TYPE::ARROWICONSCRIPT)
{
}

CArrowIconScript::~CArrowIconScript()
{
}

void CArrowIconScript::begin()
{
}

void CArrowIconScript::tick()
{
}

void CArrowIconScript::MagicActive(bool _bActive)
{
	wstring wstrArrowImageName;
	CPlayerScript* pScript = CLevelMgr::GetInst()->FindObjectByName(L"Player")->GetScript<CPlayerScript>();
	UINT magicState = pScript->GetUseMagic();
	if (magicState == (UINT)PLAYER_MAGIC::ARROW)
	{
		Transform()->SetRelativePos(Vec3(-652.f, 331.f, 0.f));
		Transform()->SetRelativeScale(Vec3(100.f, 116.f, 0.f));

		wstrArrowImageName = L"texture\\HUD\\ArrowActive.png";
	}
	else
	{
		Transform()->SetRelativePos(Vec3(-652.f, 323.f, 0.f));
		Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
		wstrArrowImageName = L"texture\\HUD\\ArrowDA.png";
	}
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(wstrArrowImageName, wstrArrowImageName, 0));
}

