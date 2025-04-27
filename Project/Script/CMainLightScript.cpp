#include "pch.h"
#include "CMainLightScript.h"
#include <Engine/CDetourMgr.h>

CMainLightScript::CMainLightScript()
	: CScript((UINT)SCRIPT_TYPE::MAINLIGHTSCRIPT)
	, m_pPlayer(nullptr)
{
}

CMainLightScript::~CMainLightScript()
{
}

void CMainLightScript::begin()
{
	if(nullptr == m_pPlayer)
		m_pPlayer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
}

void CMainLightScript::tick()
{
	if(nullptr != m_pPlayer)
	{
		Vec3 vPlayerPos = m_pPlayer->Transform()->GetWorldPos();
		Vec3 vLightPos = vPlayerPos + Vec3(-1000.f, 4000.f, 0.f);
		Transform()->SetRelativePos(vLightPos);
	}
}
