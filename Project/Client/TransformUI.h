#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
private: 
    bool    m_bShowEdieWave;
    wstring m_wstrPrefabName;
    Vec3    m_vSpawnPos;

public:
    virtual int render_update() override;
    void ShowWaveEditor();

public:
    TransformUI();
    ~TransformUI();
};

