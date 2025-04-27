#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CWeightMapShader.h"

enum class COMPUTESHADER
{
    HEIGHT,
    RAYCAST,
    WEIGHT,
};

class CLandScape :
    public CRenderComponent
{
private:
    UINT                    m_iFaceX;
    UINT                    m_iFaceZ;

    Ptr<CTexture>           m_HeightMap;    // 파일 로딩으로 가져온 높이텍스쳐

    Vec2                    m_vBrushScale;  // 브러쉬 크기(전체 지형대비 크기)
    float                   m_fVelocity;    // 브러쉬 반영 속도
    Ptr<CTexture>           m_pBrushTex;    // 브러쉬 텍스쳐
    UINT                    m_iBrushIdx;

    Ptr<CRaycastShader>     m_pCSRaycast;   // 마우스 피킹 쉐이더
    CStructuredBuffer*      m_pCrossBuffer; // 마우스 피킹 지점 정보를 받는 버퍼

    Ptr<CHeightMapShader>   m_pCSHeightMap; // 높이맵 쉐이더
    Ptr<CTexture>           m_pHeightMap;   // 높이맵 텍스쳐

    Ptr<CWeightMapShader>   m_pCSWeightMap;     // 가중치 쉐이더
    CStructuredBuffer*      m_pWeightMapBuffer; // 가중치 저장 버퍼
    UINT                    m_iWeightWidth;     // 가중치 버퍼의 가로세로 행렬 수
    UINT                    m_iWeightHeight;    // 증가시킬 가중치 부위
    UINT                    m_iWeightIdx;       // 텍스쳐 배열 중 사용할 텍스쳐 

    LANDSCAPE_MOD           m_eMod;             // 지형 툴모드에서 상태값

    Ptr<CTexture>           m_pTileArrTex;      // 타일 배열 텍스쳐

    Vec2                    m_vTessFactorDist;
    int                     m_iMaxTessFactorLevel;


  

public:
    LANDSCAPE_MOD GetCurMod() { return m_eMod; }
    void SetMod(LANDSCAPE_MOD _tMod) { m_eMod = _tMod; }
    
    void SetFace(UINT _iFaceX, UINT _iFaceZ);
    Vec2 GetFace() { return Vec2(m_iFaceX, m_iFaceZ); }

    Ptr<CTexture>& GetBrushTex() { return m_pBrushTex; }
    void SetBrushTex(Ptr<CTexture> _pBrushTex) { m_pBrushTex = _pBrushTex; }

    Vec2 GetBrushScale() { return m_vBrushScale; }
    void SetBrushScale(Vec2 _vScale) { m_vBrushScale = _vScale; }

    void SetVelocity(float _fVelocity) { m_fVelocity = _fVelocity; }
    float GetVelocity() { return m_fVelocity; }

    UINT GetBrushIdx() { return m_iBrushIdx; }
    void SetBrushIdx(UINT _iBrushIdx) { m_iBrushIdx = _iBrushIdx; }

    Ptr<CTexture>& GetHeightMap() { return m_pHeightMap; }
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap; }

    UINT GetWeightIdx() { return m_iWeightIdx; }
    void SetWeightIdx(UINT _iWeight) { m_iWeightIdx = _iWeight; }

    Ptr<CTexture>& GetTileArrTex() { return m_pTileArrTex; }

    Vec2 GetTessFactorDist() { return m_vTessFactorDist; }
    void SetTessFactorDist(Vec2 _vDist) { m_vTessFactorDist = _vDist; }

    int GetMaxTessFactorLevel() { return m_iMaxTessFactorLevel; }
    void SetMaxTessFactorLevel(int _iLevel) { m_iMaxTessFactorLevel = _iLevel; }

    virtual void begin() override;
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;
    virtual void render_depthmap() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    void init();
    void CreateMesh();
    void CreateComputeShader();
    void CreateTexture();

    void Raycasting();

    void ComputeShaderSaveLoad(Ptr<CComputeShader> _pShader, COMPUTESHADER _tShader, wstring _strKey);

    CLONE(CLandScape);

public:
    CLandScape();
    ~CLandScape();
};

