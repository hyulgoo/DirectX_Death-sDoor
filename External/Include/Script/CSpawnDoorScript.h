#pragma once
#include <Engine/CScript.h>

// 몬스터를 스폰시키는 Door의 스크립트

class CSpawnDoorScript :
    public CScript
{
private:
    wstring     m_strSpawnMstName;  // 자신이 스폰스킬 몬스터의 이름
    float       m_fDelay;           // 몬스터를 스폰시킬 타이밍
    bool        m_bSpawn;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetSpawnMst(wstring _strSpawnNum) { m_strSpawnMstName = _strSpawnNum; }
    void SetDelay(float _Delay) { m_fDelay = _Delay; }

    CLONE(CSpawnDoorScript);
public:
    CSpawnDoorScript();
    ~CSpawnDoorScript();
};

