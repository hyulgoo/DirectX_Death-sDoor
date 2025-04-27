#include "pch.h"
#include "CThreadMgr.h"

CThreadMgr::CThreadMgr()
{
}

CThreadMgr::~CThreadMgr()
{
	{
		auto	iter = m_mapThread.begin();
		auto	iterEnd = m_mapThread.end();

		for (; iter != iterEnd; ++iter)
		{
			iter->second->Stop();

			if (iter->second)
			{
				delete iter->second;
				iter->second = nullptr;
			}
		}

		m_mapThread.clear();
	}

	{
		auto	iter = m_mapCriticalSection.begin();
		auto	iterEnd = m_mapCriticalSection.end();

		for (; iter != iterEnd; ++iter)
		{
			// 전역함수를 호출하겠다는 의미이다.
			::DeleteCriticalSection(iter->second);

			if (iter->second)
			{
				delete iter->second;
				iter->second = nullptr;
			}
		}

		m_mapCriticalSection.clear();
	}
}

bool CThreadMgr::init()
{
	// 로딩용 크리티컬 섹션 생성
	CreateCriticalSection("Loading");

	return true;
}

bool CThreadMgr::Suspend(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
		return false;

	Thread->Suspend();

	return true;
}

bool CThreadMgr::Resume(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
		return false;

	Thread->Resume();

	return true;
}

bool CThreadMgr::ReStart(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
		return false;

	Thread->ReStart();

	return true;
}

bool CThreadMgr::Delete(const std::string& Name)
{
	auto	iter = m_mapThread.find(Name);

	if (iter == m_mapThread.end())
		return false;

	iter->second->Stop();

	if (iter->second)
	{
		delete iter->second;
		iter->second = nullptr;
	}

	m_mapThread.erase(iter);

	return true;
}

bool CThreadMgr::Start(const std::string& Name)
{
	CThread* Thread = FindThread(Name);

	if (!Thread)
		return false;

	Thread->Start();

	return true;
}

CThread* CThreadMgr::FindThread(const std::string& Name)
{
	auto	iter = m_mapThread.find(Name);

	if (iter == m_mapThread.end())
		return nullptr;

	return iter->second;
}

bool CThreadMgr::CreateCriticalSection(const std::string& Name)
{
	CRITICAL_SECTION* Crt = FindCriticalSection(Name);

	if (Crt)
		return false;

	Crt = new CRITICAL_SECTION;

	InitializeCriticalSection(Crt);

	m_mapCriticalSection.insert(std::make_pair(Name, Crt));

	return true;
}

bool CThreadMgr::DeleteCriticalSection(const std::string& Name)
{
	auto	iter = m_mapCriticalSection.find(Name);

	if (iter == m_mapCriticalSection.end())
		return false;

	::DeleteCriticalSection(iter->second);

	if (iter->second)
	{
		delete iter->second;
		iter->second = nullptr;
	}

	m_mapCriticalSection.erase(iter);

	return true;
}

CRITICAL_SECTION* CThreadMgr::FindCriticalSection(const std::string& Name)
{
	auto	iter = m_mapCriticalSection.find(Name);

	if (iter == m_mapCriticalSection.end())
		return nullptr;

	return iter->second;
}
