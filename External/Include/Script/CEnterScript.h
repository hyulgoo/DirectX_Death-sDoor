#pragma once
#include <Engine/CScript.h>

class CColiider3D;

// Coliider를 가지고 방의 입구에 위치함
// Player와 충돌하면 자신을 삭제하며 문을 닫고 SpawnMgr에 자신이 가진 번호의 방을 활성화시키게 함.
class CEnterScript :
    public CScript
{
private:
    int      m_iRoomNum;

public:
    virtual void begin() override;
    virtual void tick() override {};

public:
    void SetRoomNum(int _iRoomNum) { m_iRoomNum = _iRoomNum; }

    virtual void BeginOverlap(CCollider3D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CEnterScript);
public:
    CEnterScript();
    ~CEnterScript();

    friend class CSpawnMgr;
};

