#include "pch.h"
#include "CLoadingThread.h"
#include "CLevelSaveLoadInScript.h"

#include <Engine\CPathMgr.h>
#include <Engine\DataStream.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLayer.h>
#include <Engine\CEventMgr.h>


CLoadingThread::CLoadingThread() :
	m_bLoadComplete(false)
	, m_LoadLevelThreadScript(nullptr)
{
}

CLoadingThread::~CLoadingThread()
{
}

void CLoadingThread::Run()
{
	if (m_bLoadComplete)
		return;

	// Level 불러오기
	CLevel* pLoadedLevel = CLevelSaveLoadInScript::Stop(m_LevelPath, LEVEL_STATE::STOP);
	pLoadedLevel->SetLevelType((int)g_tNextLevel);
	m_LoadLevelThreadScript->SetLoadLevel(pLoadedLevel);
	m_bLoadComplete = true;
}
