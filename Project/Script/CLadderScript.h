#pragma once
#include <Engine/CScript.h>

class CLadderScript :
    public CScript
{
private:
    CGameObject*    m_pInterectionImage;
    float           m_fHeight;

public:
    virtual void begin() override {};
    virtual void tick() override {};

    void SetHeight(float _fHeight) { m_fHeight = _fHeight; }
    float GetHeight() { return m_fHeight; }

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    CLONE(CLadderScript);

public:
    CLadderScript();
    ~CLadderScript();
};

