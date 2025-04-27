#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;
class CTexture;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*          m_ParticleBuffer;
    CStructuredBuffer*          m_RWBuffer;
    CStructuredBuffer*          m_ModuleDataBuffer;
    
    tParticleModule             m_ModuleData;
    Ptr<CParticleUpdateShader>  m_UpdateCS;

    Ptr<CTexture>               m_Tex;
   
    float                       m_AccTime;

    int                         m_iEmissive;

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;


    void SetModuleData(tParticleModule _ModuleData) {m_ModuleData = _ModuleData;}
    tParticleModule GetModuleData() { return m_ModuleData; }

    void SetTex(Ptr<CTexture> _Tex) { m_Tex = _Tex; }
    Ptr<CTexture> GetTex() { return m_Tex; }

    void SpawnModule(int _iMaxParticle, int _iSpawnRate, Vec3 _vSpawnColor, Vec3 vSpawnMinScale
        , Vec3 vSpawnMaxScale, Vec3 _vSpawnBoxScale, float _fMinLifeTime, float _fMaxLifeTime);
    void ColorChangeModule(Vec3 _vStartColor, Vec3 _vEndColor);
    void ScaleChangeModule(float _vStartScale, float _vEndScale);
    void AddVelocityModule(float _fSpeed, int _iVelocityType, Vec3 _vVelocityDir, float _fAngle);
    void DragModule(float _fStartDrag, float _fEndDrag);
    void RandomForceModule(float _fForceTerm, float _fForce);
    void VelocityAlignmentModule();
    void VelocityScaleModule(float _fMaxSpeed, Vec3 _vMaxVelocityScale);
    void AnimationModule(int _iFrmCount, int _iXCount, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vOffset);
    void SetFollow() { m_ModuleData.Space = 1; }
    void SetEmissive(bool _bEmissive) { m_iEmissive = _bEmissive; }
    bool GetEmissive() { return m_iEmissive; }

    void OnOff(bool _Onoff);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _Other);
    ~CParticleSystem();
};

