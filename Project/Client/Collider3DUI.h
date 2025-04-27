#pragma once
#include "ComponentUI.h"
class Collider3DUI :
    public ComponentUI
{
private:
    Vec3 pPos;
    float pRadius;
    bool createSphere;
    bool setRenderRigidbody;
public:
    virtual int render_update() override;

public:
    Collider3DUI();
    ~Collider3DUI();
};

