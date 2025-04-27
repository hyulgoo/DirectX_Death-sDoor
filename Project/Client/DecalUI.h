#pragma once
#include "ComponentUI.h"

class DecalUI :
    public ComponentUI
{
private:
public:
    virtual int render_update() override;

public:
    DecalUI();
    ~DecalUI();
};

