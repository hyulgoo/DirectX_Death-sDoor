#pragma once
#include "ComponentUI.h"
class LandScapeUI :
    public ComponentUI
{
private:
    
public:
    virtual int render_update() override;
    void SelectTexture(DWORD_PTR _Key);
public:
    LandScapeUI();
    ~LandScapeUI();
};

