#pragma once
#include "ComponentUI.h"
class Light3DUI :
    public ComponentUI
{
private:
    tLightInfo     m_LightInfo;

public:
    virtual int render_update() override;

public:
    Light3DUI();
    ~Light3DUI();
};

