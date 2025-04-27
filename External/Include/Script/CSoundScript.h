#pragma once
#include <Engine/CSound.h>
#include <Engine/CScript.h>
class CSoundScript :
    public CScript
{
private:
    Ptr<CSound>     m_pSound;
    float           m_fVolume;
    int             m_iIdx;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    Ptr<CSound> AddSound(wstring _filename,int _repeat, float _volume = 0.5f, bool _bOverlap = true);
    void SetIndex(int _iIdx) { m_iIdx = _iIdx; }
    int GetIndex() { return m_iIdx; }

    CLONE(CSoundScript);

public:
    CSoundScript();
    ~CSoundScript();
};

