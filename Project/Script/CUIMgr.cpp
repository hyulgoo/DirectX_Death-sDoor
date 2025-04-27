#include "pch.h"
#include "CUIMgr.h"
#include "CLevelSaveLoadInScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include "CPlayerScript.h"
#include "CIconScript.h"
#include "CArrowIconScript.h"
#include "CBombIconScript.h"
#include "CFireIconScript.h"
#include "CHookIconScript.h"
#include "CHPIconScript.h"
#include "CMPIconScript.h"

CUIMgr::CUIMgr()
	: m_arrMagicUI{nullptr,}
	, m_arrHPMPUI{nullptr,}
	, m_bActive(false)
	, m_pFrameUI(nullptr)
{
	CreateUI();
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::UpgradePlayer(PLAYER_UPGRADE _tUpgradeType)
{
	CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player")->GetScript< CPlayerScript>()->Upgrade(_tUpgradeType);
}

void CUIMgr::ActiveMagic(int _iCurMagic)
{
	SetMP(g_tPlayerStat.MP, _iCurMagic);
	for (UINT i = 0; i < (UINT)PLAYER_UPGRADE::END; ++i)
	{
		bool bActive = i == _iCurMagic ? true : false;
		switch ((PLAYER_MAGIC)i)
		{
		case PLAYER_MAGIC::ARROW:
			m_arrMagicUI[i]->GetScript<CArrowIconScript>()->MagicActive(bActive);
			break;
		case PLAYER_MAGIC::FIRE:
			m_arrMagicUI[i]->GetScript<CFireIconScript>()->MagicActive(bActive);
			break;
		case PLAYER_MAGIC::BOMB:
			m_arrMagicUI[i]->GetScript<CBombIconScript>()->MagicActive(bActive);
			break;
		case PLAYER_MAGIC::HOOK:
			m_arrMagicUI[i]->GetScript<CHookIconScript>()->MagicActive(bActive);
			break;
		}
	}
}

void CUIMgr::SetHP(int _iCurHP)
{
	if(!m_arrHPMPUI[(UINT)HUDUI::HP]->GetScript<CHPIconScript>())
		CreateUI();
	m_arrHPMPUI[(UINT)HUDUI::HP]->GetScript<CHPIconScript>()->SetHP(_iCurHP);
}

void CUIMgr::SetMP(int _iCurMP, int _iCurMagic)
{
	if (!m_arrHPMPUI[(UINT)HUDUI::MP]->GetScript<CMPIconScript>())
	CreateUI();
	m_arrHPMPUI[(UINT)HUDUI::MP]->GetScript<CMPIconScript>()->SetMP(_iCurMP, (PLAYER_MAGIC)_iCurMagic);
}

void CUIMgr::CreateUI()
{
	wstring wstrPrefabName;
	for (int i = 0; i < (UINT)PLAYER_MAGIC::END; ++i)
	{
		switch ((PLAYER_MAGIC)i)
		{
		case PLAYER_MAGIC::ARROW:
			wstrPrefabName = L"prefab\\Arrow_Icon.prefab";
			break;
		case PLAYER_MAGIC::FIRE:
			wstrPrefabName = L"prefab\\Fire_Icon.prefab";
			break;
		case PLAYER_MAGIC::BOMB:
			wstrPrefabName = L"prefab\\Bomb_Icon.prefab";
			break;
		case PLAYER_MAGIC::HOOK:
			wstrPrefabName = L"prefab\\Hook_Icon.prefab";
			break;
		}
		m_arrMagicUI[i] = CLevelSaveLoadInScript::SpawnandReturnPrefab(wstrPrefabName, (int)LAYER::UI, Vec3(0.f));
	}
		m_arrHPMPUI[(UINT)HUDUI::HP] = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\HP_Icon.prefab", (int)LAYER::UI, Vec3(0.f));

		m_arrHPMPUI[(UINT)HUDUI::MP] = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\MP_Icon.prefab", (int)LAYER::UI, Vec3(0.f));
}
