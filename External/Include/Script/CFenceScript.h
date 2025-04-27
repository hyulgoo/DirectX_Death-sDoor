#pragma once
#include <Engine/CScript.h>
class CFenceScript :
    public CScript
{
private:
    int                     m_iRoomNum;
    physx::PxRigidStatic*   m_pStatic;
    Vec3                    m_vStartPos;
    float                   m_fMoveDistance;
    bool                    m_bActive;
    bool                    m_bOpen;

public:
    virtual void begin() override;
    virtual void tick() override;

    void SetRoomNum(int _iRoomNum) { m_iRoomNum = _iRoomNum; }
    void ActivateFence(bool _bOpen);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CFenceScript);

public:
    CFenceScript();
    ~CFenceScript();

    friend class CSpawnMgr;
};

