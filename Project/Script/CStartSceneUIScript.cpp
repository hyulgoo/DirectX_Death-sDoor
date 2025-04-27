#include "pch.h"
#include "CStartSceneUIScript.h"
#include "CLevelSaveLoadInScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine\CEventMgr.h>

CStartSceneUIScript::CStartSceneUIScript() :
	CScript(SCRIPT_TYPE::STARTSCENEUISCRIPT)
{
}

CStartSceneUIScript::~CStartSceneUIScript()
{
}

void CStartSceneUIScript::tick()
{
	if (KEY_TAP(KEY::LBTN))
	{
		g_tNextLevel = LEVEL_TYPE::HALL;

		CLevel* NewLevel = CLevelSaveLoadInScript::Stop(L"Level\\Hall.lv", LEVEL_STATE::STOP);
		NewLevel->SetName(L"Hall");
		NewLevel->SetLevelType((int)LEVEL_TYPE::HALL);
		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)NewLevel;
		evn.lParam = (DWORD_PTR)NewLevel->GetLevelType();
		CEventMgr::GetInst()->AddEvent(evn);
	}
}
