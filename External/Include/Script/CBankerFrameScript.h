#pragma once
#include <Engine/CScript.h>

class CBankerFrameScript :
    public CScript
{
private:
    int m_iCount;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CBankerFrameScript);
public:
    CBankerFrameScript();
    ~CBankerFrameScript();
};

