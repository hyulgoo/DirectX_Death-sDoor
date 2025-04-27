#pragma once
#include <Engine\CScript.h>
class CGameCameraScript :
    public CScript
{
private:
    CGameObject*    m_pTarget;
    float           m_fMoveTime;
    float           m_fPrevMoveTime;
    float           m_fDiffer;
    float           m_fTargetScale;
    float           m_fPrevScale;
    Vec3            m_vDistance;
    // CameraShake¿ë ¸â¹ö
    Vec2		    m_vOffset;
    float		    m_fAccTime;
    float		    m_fMaxTime;
    float		    m_fRange;
    float		    m_fShakeSpeed;
    float		    m_fShakeDir;
    bool		    m_bCamShake;
    bool            m_bCutSceneView;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;
    void SetInvincible(float _fMoveTime) { m_fMoveTime = _fMoveTime; }
    void SetMoveCamera(float _vTargetScale, float _fTime);
    Vec3 GetDistance() { return m_vDistance; }
    
    void SetTarget(CGameObject* _pTarget) { m_pTarget = _pTarget; }
    void SetTargetPlayer(){ m_pTarget = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player"); }
    void SetCutSceneView(bool _bCutSceneView);

    void CameraShake(float _fRange, float _fShackSpeed, float _fTerm);
    void ShackCamera();

    CLONE(CGameCameraScript);

public:
    CGameCameraScript();
    ~CGameCameraScript();
};

