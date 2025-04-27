#pragma once
#include "ResUI.h"
#include <Engine/CSound.h>

class SoundUI :
    public ResUI
{
private:
    CSound* m_sound;
    FMOD::Channel* pChannel;
    int idx;
    float m_volume;
public:
    virtual int render_update() override;

public:
    SoundUI();
    ~SoundUI();
};

