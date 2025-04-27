#pragma once
#include "ComponentUI.h"

class RigidbodyUI :
    public ComponentUI
{
private:
    bool m_IsStaticCube;
    bool m_IsSphere;
public:
    virtual int render_update() override;

public:
    RigidbodyUI();
    ~RigidbodyUI();
};

