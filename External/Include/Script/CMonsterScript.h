#pragma once
#include <Engine\CScript.h>

class CStateScript;

class CMonsterScript :
    public CScript
{
private:
    bool            m_bPaperBurnEffect;     // PaperBurn 효과를 줄지.
    bool            m_bSendDeadTime;        // 사망시간을 이미 보냈는지. 
    float           m_fDeathTime;           // 사망 시각.
    float           m_fLastHitTime;         // 마지막으로 피격된 시각.
    bool            m_bFixPos;              // 위치 고정 여부.
    Vec3            m_vFixedPos;            // 고정 위치.
    bool            m_bSpawnByDoor;

protected:
	CStateScript*   m_pStateScript;
	CGameObject*    m_pPlayer;
	bool            m_bDetect;

public:
	const bool      IsDetect() const { return m_bDetect; }
	CGameObject*    GetPlayer() const { return m_pPlayer; }
    void            SetDetect(const bool _bDetect) { m_bDetect = _bDetect; }
    bool            GetPaperBurnEffect() const { return m_bPaperBurnEffect;}
    void            SetPaperBurnEffect(const bool value) { m_bPaperBurnEffect = value;}
    void            SetLastHitTime() { m_fLastHitTime = GlobalData.tAccTime; }
    void            SpawnByDoor() { m_bSpawnByDoor = true; }
    void            SetFixPosition(const bool _bFixPos)
    { 
        m_bFixPos = _bFixPos;
        m_vFixedPos = Transform()->GetWorldPos();
    }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    CLONE(CMonsterScript);
public:
    CMonsterScript(UINT SCRIPT_TYPE);
    ~CMonsterScript();  
};


