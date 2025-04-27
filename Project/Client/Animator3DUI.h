#pragma once
#include "ComponentUI.h"
class Animator3DUI :
    public ComponentUI
{
public:
    int                     m_iClipCount;
    vector<tMTAnimClip>     m_vecAnimClip;
    bool                    m_bShowAnimInfo;
    bool                    m_bMakeAnimInfo;

public:
    virtual int render_update() override;

public:
    Animator3DUI();
    ~Animator3DUI();
};

