#pragma once

#include <Engine\CThread.h>
#include <Engine\CLevel.h>
#include <Engine\CThreadQueue.h>

#include "CLoadLevelThreadScript.h"


enum class ELoadingHeader
{
	LoadingRate
};

class CLoadingThread :
	public CThread
{
	friend class CThreadMgr;

protected:
	CLoadingThread();
	virtual ~CLoadingThread();

protected:
	wstring						m_LevelPath;
	bool						m_bLoadComplete;
	CThreadQueue				m_Queue;
	CLoadLevelThreadScript*		m_LoadLevelThreadScript;	// 이 CLoadingThread를 생성한 CLoadLevelThreadScript.

public:
	void SetLoadLevelThreadScript(CLoadLevelThreadScript* _CLoadLevelThreadScript)
	{
		m_LoadLevelThreadScript = _CLoadLevelThreadScript;
	}

	CThreadQueue* GetQueue()
	{
		return &m_Queue;
	}

	bool IsLoadComplete()	const
	{
		return m_bLoadComplete;
	}

public:
	void SetLoadingLevelPath(const wstring& LevelPath)
	{
		m_LevelPath = LevelPath;
	}

public:
	virtual void Run();
};

