#pragma once
#include "CState.h"
class CCrowBossSliding :
    public CState
{
private:
    vector<CGameObject*>    m_vecHook;
    queue<Vec3>             m_qStartPos;        // Hook을 보스에서 날리지 않는 경우 슬라이딩을 시작할 위치
    queue<Vec3>             m_qTargetPos;       // Hook을 여러 개 날렸을 때 차례대로 슬라이딩 할 위치를 받아둠
    Vec3                    m_vDirtoTarget;     
    int                     m_iSliderCount;     // 슬라이딩 횟수
    CHAINPATERN             m_tChainPatern;
   
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetHook(vector<CGameObject*> _vecHook) { if (m_vecHook.empty()) m_vecHook = _vecHook; }
    void AddTargetPos(Vec3 _vTargetPos, Vec3 _vStartPos = {});
    void SetPatern(CHAINPATERN _tPatern) { m_tChainPatern = _tPatern; }
    void SlidingToTargetPos();
    void ArriveToTarget();

    CLONE(CCrowBossSliding);

public:
    CCrowBossSliding();
    ~CCrowBossSliding();

};

