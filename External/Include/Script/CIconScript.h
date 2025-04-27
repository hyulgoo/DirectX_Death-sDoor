#pragma once
#include <Engine/CScript.h>
class CIconScript :
    public CScript
{
public:
    virtual void MagicActive(bool _bActive) {};
    virtual void SetHP(int iCurHP) {};
    virtual void SetMP(int iCurMP, PLAYER_MAGIC iCurMagic) {};
    virtual void Disable();

    CLONE(CIconScript);

public:
    CIconScript(UINT SCRIPT_TYPE);
    ~CIconScript();
};

