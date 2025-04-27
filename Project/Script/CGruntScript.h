#pragma once
#include "CMonsterScript.h"

class CGruntScript :
    public CMonsterScript
{
private:
    Vec3    m_vPlayerPos;
    Vec3    m_vMonsterToPlayerDir;
    float   m_fPlayerDistance;
    float   m_fAttackRange;     // 공격 범위. 
    int     m_iNailAttackCount;  // 손톱 공격 횟수. 2회시 점프 공격.
    bool    m_bStarePlayer;
    float   m_fPrevHP;          // 이전 HP. HP가 줄었을 때, 데미지 사운드를 재생하기 위해 사용.

public:
    Vec3 GetPlayerPos() { return m_vPlayerPos; }
    Vec3 GetMonsterToPlayerDir() { return m_vMonsterToPlayerDir; }
    float GetPlayerDistance() { return m_fPlayerDistance; }

    float GetAttackRange() { return m_fAttackRange; }

    int GetNailAttackCount() { return m_iNailAttackCount; }
    void CountNailAttack();

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

    CLONE(CGruntScript);

public:
    CGruntScript();
    CGruntScript(const CGruntScript& _Other);
    ~CGruntScript();
};

