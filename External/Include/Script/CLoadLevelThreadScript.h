#pragma once

#include <Engine\CScript.h>
#include <Engine\CLevel.h>

class CLoadLevelThreadScript :
    public CScript
{
private:
    class CLoadingThread*   m_LoadingThread;
    class CThreadQueue*     m_LoadingQueue;
    CLevel*                 m_LoadLevel;

public:
    void SetLoadLevel(CLevel* _LoadLevel)
    {
        m_LoadLevel = _LoadLevel;
    }

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _FILE) override;

private:
    CLONE(CLoadLevelThreadScript);
public:
    CLoadLevelThreadScript();
    ~CLoadLevelThreadScript();
};

