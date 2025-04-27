#pragma once
#include "CState.h"

enum class SLASH
{
    LEFT,
    RIGHT,
    END,
};

class CPlyAttack :
    public CState
{
private:
    CGameObject*    m_pSlash[(UINT)SLASH::END];    // 공격 시 출력되는 SlashMesh 
    UINT            m_iAttackCount;         // 현재 공격횟수(총 3회까지)
    Vec3            m_vSlashPos;
    float           m_fRange;
    float           m_fAcctime;
    float           m_fDelay;
    float           m_fAfterAttack;         // 공격모션이 끝난 후 시간
    float           m_fTimeToIdle; // 공격모션이 끝난 후 Idle로 되돌아가는 시간
    float           m_fAttackDir;
    Vec3            m_vMouseDir;

    // 
    float           m_fSlashStartTime;      // 이번 Slash 이펙트 생성 시각.
    
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    UINT GetAttackCount() { return m_iAttackCount; }
    void CalcDir();
    void Slash();
    void SetSlashScale(bool _bOn, SLASH _tDir);

    CLONE(CPlyAttack);

public:
    CPlyAttack();
    ~CPlyAttack();
};

