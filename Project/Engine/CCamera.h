#pragma once
#include "CComponent.h"
#include "CFrustum.h"

class CCamera :
    public CComponent
{
private:
    CFrustum                m_Frustum;

    float                   m_fWidth;       // 투영 가로길이
    float                   m_fAspectRatio;
    float                   m_fScale;       // Orthograpic 에서 사용하는 카메라 배율
    float                   m_fFar;    

    PROJ_TYPE               m_ProjType;

    Matrix                  m_matView;
    Matrix                  m_matViewInv;
    Matrix                  m_matProj;
    Matrix                  m_matProjInv;

    UINT                    m_iLayerMask;

    int                     m_iCamIdx;  // 카메라 우선순위

    tRay                    m_ray;      // 마우스 방향을 향하는 직선


    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D;	    // Deferred
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F;	    // Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj;		    // Single Object

    //vector<CGameObject*>    m_vecDeferred;
    vector<CGameObject*>    m_vecDecal;

    //vector<CGameObject*>    m_vecOpaque;
    //vector<CGameObject*>    m_vecMask;

    vector<CGameObject*>    m_vecTransparent;    
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;

    vector<CGameObject*>    m_vecDynamicShadow;     // 동적 그림자 물체

    // 
    bool                    m_bWaterCamera;
    Ptr<CTexture>           m_WaterCamTex;

public:
    void SetWaterCamera(bool _bWaterCamera) { m_bWaterCamera = _bWaterCamera; }

    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetScale(float _fScale) { m_fScale = _fScale; }
    float GetScale() { return m_fScale; }

    void SetFar(float _Far) { m_fFar = _Far; }
    float GetFar() { return m_fFar; }

    void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
    void SetAspectRatio(float _fRatio) { m_fAspectRatio = _fRatio; }

    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll(bool _Visible);

    void SetCameraIndex(int _idx);
    void SetDrawDebugShape(bool _bDraw) { m_Frustum.SetDrawDebugShape(_bDraw); }

    const tRay& GetRay() { return m_ray; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetViewMatInv() { return m_matViewInv; }
    const Matrix& GetProjMat() { return m_matProj; }
    const Matrix& GetProjMatInv() { return m_matProjInv; }

public:
    void SortObject();
    void SortShadowObject();
    void render();
    void render_depthmap();
    void render_water();

public:
    virtual void begin() override;
    virtual void finaltick() override;

protected:
    void CalRay();  // 마우스 방향으로 광선 연산


private:
    void clear();
    void render_deferred();
    void render_decal();

    // 가우시안 블러용
    void render_blur();

    void render_merge();

    void render_forward();
    //void render_opaque();
    //void render_mask();
    void render_transparent();
    void render_postprocess();
    void render_ui();

    void CalcViewMat();
    void CalcProjMat();


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCamera);
public:    
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();
};

