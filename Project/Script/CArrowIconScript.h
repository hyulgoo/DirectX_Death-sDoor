#pragma once
#include "CIconScript.h"
class CArrowIconScript :
    public CIconScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void MagicActive(bool _bActive) override;

    CLONE(CArrowIconScript);

public:
    CArrowIconScript();
    ~CArrowIconScript();
};

