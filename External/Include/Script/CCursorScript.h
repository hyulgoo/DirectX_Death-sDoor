#pragma once
#include <Engine/CScript.h>
class CCursorScript :
    public CScript
{
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CCursorScript);

    CCursorScript();
    ~CCursorScript();
};

