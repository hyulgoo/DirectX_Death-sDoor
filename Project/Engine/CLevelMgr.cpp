#include "pch.h"
#include "CLevelMgr.h"
#include "CPhysXMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CPhysXMgr.h"
#include "CDetourMgr.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;
	m_pCurLevel->ChangeState(LEVEL_STATE::STOP);	
}

void CLevelMgr::tick()
{
	m_pCurLevel->clear();

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->tick();

		CPhysXMgr::GetInst()->tick();
	}

	m_pCurLevel->finaltick();

	CPhysXMgr::GetInst()->finaltick();
}


CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);	
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _vec)
{
	m_pCurLevel->FindObjectByName(_strName, _vec);
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (g_tPlayerStat.HP == 0)
	{
		Stat PlayerStat;
		PlayerStat.Attack = 50.f;
		PlayerStat.Attack_Speed = 0.4f;
		PlayerStat.HP = 4;
		PlayerStat.Speed = 150.f;
		PlayerStat.Spell_Power = 40.f;
		PlayerStat.MP = 4;
		PlayerStat.Max_MP = 4;

		g_tPlayerStat = PlayerStat;
	}

	if (nullptr != m_pCurLevel)
	{
		delete m_pCurLevel;
		m_pCurLevel = nullptr;
	}
	
	m_pCurLevel = _NextLevel;
	CPhysXMgr::GetInst()->ChangeLevel(((LEVEL_TYPE)m_pCurLevel->GetLevelType()));
	CDetourMgr::GetInst()->ChangeLevel(((LEVEL_TYPE)m_pCurLevel->GetLevelType()));
}
