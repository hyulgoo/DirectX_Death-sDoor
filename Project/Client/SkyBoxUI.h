#pragma once
#include "ComponentUI.h"

class SkyBoxUI :
    public ComponentUI
{
private:
    int     m_iType;
public:
    virtual int render_update() override;

public:
    void SelectTexture(DWORD_PTR _Key);

public:
    SkyBoxUI();
    ~SkyBoxUI();
};

