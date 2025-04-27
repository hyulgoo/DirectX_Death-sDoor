#pragma once
#include <Engine/CSingleton.h>

class CRoomScript;
class CFenceScript;

struct FenceInfo
{
    int             RoomNum;
    CFenceScript*   Fence;
};

class CSpawnMgr :
    public CSingleton<CSpawnMgr>
{
    SINGLE(CSpawnMgr);
private:
    int                     m_iCurRoomNum;     // 현재 활성화 된 방의 번호
    map<int, CRoomScript*>  m_mapRoom;
    vector<FenceInfo>       m_vecFence;         // 각 번호를 가진 fence obj

public:
    void RegisterFence(int _iRoomNum, CFenceScript* _pFence);
    void RegisterRoom(int _iRoomNum, CRoomScript* _pRoom);
    void SpawnMonster(int _iRoomNum);
    void ActivateFence(int _iRoomNum, bool _bOpen);

    void ReduceMonsterCount();
    void ReduceGimmickCount();

    void Clear();
};

