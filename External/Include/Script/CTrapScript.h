#pragma once
#include <Engine/CScript.h>
class CTrapScript :
    public CScript
{
private:
    int m_iTrapNum;

private:
    void SetTrapNum(int _trapNum) { m_iTrapNum = _trapNum; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;

    CLONE(CTrapScript);
public:
    CTrapScript();
    ~CTrapScript();

};
