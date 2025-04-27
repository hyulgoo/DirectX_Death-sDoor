#pragma once
#include "CMonsterScript.h"

class CBazookaScript :
    public CMonsterScript
{
private:
    Vec3    m_PlayerPos;
    Vec3    m_MonsterToPlayerDir;
    float   m_fPlayerDistance;
    float   m_fMeleeRange;      // 근접 공격 범위
    float   m_fRunAwayRange;    // m_fMeleeRange ~ m_fRunAwayRange면 도망감
    float   m_fAttackRange;     // m_fRunAwayRange ~ m_fAttackRange면 조준 및 발사. 그 이상은 추적.
    bool    m_bStarePlayer;
    float   m_fPrevHP;          // 이전 HP. HP가 줄었을 때, 데미지 사운드를 재생하기 위해 사용.

public:
    Vec3 GetPlayerPos() { return m_PlayerPos; }
    Vec3 GetMonsterToPlayerDir() { return m_MonsterToPlayerDir; }
    float GetPlayerDistance() { return m_fPlayerDistance; }

    float GetMeleeRange() { return m_fMeleeRange; }
    float GetRunAwayRange() { return m_fRunAwayRange; }
    float GetAttackRange() { return m_fAttackRange; }

    bool GetStarePlayer() { return m_bStarePlayer; }
    void SetStarePlayer(bool _bStarePlayer) { m_bStarePlayer = _bStarePlayer; }


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBazookaScript);

public:
    CBazookaScript();
    CBazookaScript(const CBazookaScript& _Other);
    ~CBazookaScript();
};

