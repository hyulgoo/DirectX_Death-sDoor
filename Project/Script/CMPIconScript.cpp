#include "pch.h"
#include "CMPIconScript.h"
#include "CStateScript.h"
#include "CPlayerScript.h"

CMPIconScript::CMPIconScript() :
	CIconScript(SCRIPT_TYPE::MPICONSCRIPT)
{
}

CMPIconScript::~CMPIconScript()
{
}

void CMPIconScript::begin()
{
	// µ¿Àû ÀçÁú »ý¼º.
	Transform()->SetRelativePos(Vec3(-465.f, 319.f, 0.f));
	Transform()->SetRelativeScale(Vec3(154.f, 43.f, 0.f));
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\EGempty.png", L"texture\\HUD\\EGempty.png", 0));	
}

void CMPIconScript::tick()
{
	if (CLevelMgr::GetInst()->FindObjectByName(L"BankUIFrame"))
	{
		Transform()->SetRelativePos(Vec3(-1000.f, 0.f, 0.f));
	}
	else
	{
		Transform()->SetRelativePos(Vec3(-465.f, 319.f, 0.f));
	}

	Stat playerStatus = CLevelMgr::GetInst()->FindObjectByName(L"Player")->GetScript<CStateScript>()->GetStat();
	int energy = playerStatus.MP;
	CPlayerScript* pScript = CLevelMgr::GetInst()->FindObjectByName(L"Player")->GetScript<CPlayerScript>();
	UINT magicType = pScript->GetUseMagic();
	
}

void CMPIconScript::SetMP(int _iCurMP, PLAYER_MAGIC iCurMagic)
{
	wstring wstrMPImageName;
	//´Ù »¡°­
	if (_iCurMP >= 4 && iCurMagic == PLAYER_MAGIC::HOOK)
		wstrMPImageName = L"texture\\HUD\\EG4r0w.png";
	//2°³¸¸ Èò»ö
	else if (_iCurMP >= 4 && iCurMagic == PLAYER_MAGIC::BOMB)
		wstrMPImageName = L"texture\\HUD\\EG2r2w.png";
	// 1°³¸¸ Èò»ö
	else if (_iCurMP >= 4 && iCurMagic != PLAYER_MAGIC::BOMB && iCurMagic != PLAYER_MAGIC::HOOK)
		wstrMPImageName = L"texture\\HUD\\EG3r1w.png";
	//´Ù »¡°­
	else if (_iCurMP == 3 && iCurMagic == PLAYER_MAGIC::HOOK)
		wstrMPImageName = L"texture\\HUD\\EG3r0w.png";
	//2°³¸¸ Èò»ö
	else if (_iCurMP == 3 && iCurMagic == PLAYER_MAGIC::BOMB)
		wstrMPImageName = L"texture\\HUD\\EG1r2w.png";
	// 1°³¸¸ Èò»ö
	else if (_iCurMP == 3 && iCurMagic != PLAYER_MAGIC::BOMB && iCurMagic != PLAYER_MAGIC::HOOK)
		wstrMPImageName = L"texture\\HUD\\EG2r1w.png";
	//´Ù »¡°­
	else if (_iCurMP == 2 && iCurMagic == PLAYER_MAGIC::HOOK)
		wstrMPImageName = L"texture\\HUD\\EG2r0w.png";
	//2°³¸¸ Èò»ö
	else if (_iCurMP == 2 && iCurMagic == PLAYER_MAGIC::BOMB)
		wstrMPImageName = L"texture\\HUD\\EG0r2w.png";
	// 1°³¸¸ Èò»ö
	else if (_iCurMP == 2 && iCurMagic != PLAYER_MAGIC::BOMB && iCurMagic != PLAYER_MAGIC::HOOK)
		wstrMPImageName = L"texture\\HUD\\EG1r1w.png";
	//´Ù »¡°­
	else if (_iCurMP == 1 && iCurMagic == PLAYER_MAGIC::HOOK)
		wstrMPImageName = L"texture\\HUD\\EG1r0w.png";
	//2°³¸¸ Èò»ö
	else if (_iCurMP == 1 && iCurMagic == PLAYER_MAGIC::BOMB)
		wstrMPImageName = L"texture\\HUD\\EG1r0w.png";
	// 1°³¸¸ Èò»ö
	else if (_iCurMP == 1 && iCurMagic != PLAYER_MAGIC::BOMB && iCurMagic != PLAYER_MAGIC::HOOK)
		wstrMPImageName = L"texture\\HUD\\EG0r1w.png";
	else if (_iCurMP <= 0)
		wstrMPImageName = L"texture\\HUD\\EGempty.png";

	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(wstrMPImageName, wstrMPImageName, 0));
}
