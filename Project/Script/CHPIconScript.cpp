#include "pch.h"
#include "CHPIconScript.h"
#include "CStateScript.h"

CHPIconScript::CHPIconScript() :
	CIconScript(SCRIPT_TYPE::HPICONSCRIPT)
{
}

CHPIconScript::~CHPIconScript()
{
}

void CHPIconScript::begin()
{
	SetHP(g_tPlayerStat.HP);
	Transform()->SetRelativePos(Vec3(-444.f, 367.f, 0.f));
	Transform()->SetRelativeScale(Vec3(217.f, 41.f, 0.f));
}

void CHPIconScript::tick()
{
	if (CLevelMgr::GetInst()->FindObjectByName(L"BankUIFrame"))
	{
		Transform()->SetRelativePos(Vec3(-1000.f, 0.f, 0.f));
	}
	else
	{
		Transform()->SetRelativePos(Vec3(-444.f, 367.f, 0.f));
	}

	
}

void CHPIconScript::SetHP(int _iCurHP)
{
	wstring wstrHPImageName;
	if (_iCurHP == 4)
		wstrHPImageName = L"texture\\HUD\\HP_4.png";
	else if (_iCurHP == 3)
		wstrHPImageName = L"texture\\HUD\\HP_3.png";
	else if (_iCurHP == 2)
		wstrHPImageName = L"texture\\HUD\\HP_2.png";
	else if (_iCurHP == 1)
		wstrHPImageName = L"texture\\HUD\\HP_1.png";
	else if (_iCurHP == 0)
		wstrHPImageName = L"texture\\HUD\\HPFrame.png";

	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(wstrHPImageName, wstrHPImageName, 0));
}
