#pragma once
#include "CRenderComponent.h"

class CDecal :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_DecalTex;
    bool            m_bShowDebug;
    bool            m_Light;
    float           m_LightAlpha;

public:
    void SetAsLight(bool _bLight) { m_Light = _bLight; }
    bool IsSetAsLight() { return m_Light; }

    void SetOutputTexture(Ptr<CTexture> _Tex) { m_DecalTex = _Tex;}
    Ptr<CTexture> GetOutputTexture() { return m_DecalTex; }

    float GetAlpha() { return m_LightAlpha; }
    void SetAlpha(float _fAlpha) { m_LightAlpha = _fAlpha; }

    void SetShowDebug(bool _bShow) {m_bShowDebug = _bShow;}

    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CDecal);

public:
    CDecal();
    ~CDecal();
};

