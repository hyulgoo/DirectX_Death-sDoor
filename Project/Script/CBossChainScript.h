#pragma once
#include <Engine/CScript.h>

class CCrowBossSlidingReady;
class CCrowBossSliding;

class CBossChainScript :
    public CScript
{
private:
    CCrowBossSlidingReady*  m_pChainScript;
    CCrowBossSliding*       m_pSlidingScript;
    vector<CGameObject*>    m_vecChain;
    Vec3                    m_vThrowStartPos;       // 던지기 시작한 위치
    Vec3                    m_vThrownDir;           // 던지는 방향(거리로 사용)
    Vec3                    m_vThrownRot;           // 던지는 방향(회전각도)
    float                   m_fThrowDistance;       // 날아가야 하는 거리
    float                   m_fDistancetoTarget;    // Hookpos와의 거리
    float                   m_fChainSpacing;        // 체인 당 차지하는 공간
    float                   m_fDelay;
    bool                    m_bActive;
    bool                    m_bMulti;
    bool                    m_bSound;               // Sound 재생 여부.

private:
    void SetSlidingScript(CCrowBossSliding* _pSlidingScript) { m_pSlidingScript = _pSlidingScript; }
    void SetChainScript(CCrowBossSlidingReady* _pChainScript) { m_pChainScript = _pChainScript; }
    void SetChain(vector<CGameObject*>& _vecChain) { m_vecChain = _vecChain; }

public:
    virtual void begin() override;
    virtual void tick() override;

    void SetThrowDir(Vec3 _vThrowDir) { m_vThrownDir = _vThrowDir.Normalize(); }
    void SetThrowRot(Vec3 _vThrowRot) { m_vThrownRot = _vThrowRot; }
    void SetThrowDistance(float _fDist) { m_fThrowDistance = _fDist; }
    void SetThrowStartPos(Vec3 _vStartPos) { m_vThrowStartPos = _vStartPos; }
    void Active(bool _bActive, bool _bMulti, float _fDelay);

    vector<CGameObject*>& GetChain() { return m_vecChain; }
    Vec3 GetThrowDir() { return m_vThrownDir; }

    void PaveChain();
    void Clear();

    CLONE(CBossChainScript);

public:
    CBossChainScript();
    ~CBossChainScript();

    friend class CCrowBossSlidingReady;
    friend class CCrowBossSliding;
};

