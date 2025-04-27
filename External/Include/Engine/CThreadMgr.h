#pragma once

#include "CSingleton.h"
#include "CThread.h"

class CThreadMgr :
	public CSingleton<CThreadMgr>
{
	SINGLE(CThreadMgr);
private:
	map<string, CThread*>	m_mapThread;
	map<string, CRITICAL_SECTION*>	m_mapCriticalSection;

public:
	bool init();
	bool Suspend(const std::string& Name);
	bool Resume(const std::string& Name);
	bool ReStart(const std::string& Name);
	bool Delete(const std::string& Name);
	bool Start(const std::string& Name);
	CThread* FindThread(const std::string& Name);

	bool CreateCriticalSection(const std::string& Name);
	bool DeleteCriticalSection(const std::string& Name);
	CRITICAL_SECTION* FindCriticalSection(const std::string& Name);

public:
	template <typename T>
	T* Create(const std::string& Name)
	{
		T* Thread = (T*)FindThread(Name);

		if (Thread)
			return Thread;

		Thread = new T;

		if (!Thread->Init())
		{
			if (Thread)
			{
				delete Thread;
				Thread = nullptr;
			}

			return nullptr;
		}

		m_mapThread.insert(std::make_pair(Name, Thread));

		return Thread;
	}
};

