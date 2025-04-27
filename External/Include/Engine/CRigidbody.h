#pragma once
#include "CComponent.h"
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

class PxRigidDynamic;

class CRigidbody :
    public CComponent
{
private:
    physx::PxRigidDynamic*  m_PxRigidbody;
    SHAPE_TYPE              m_PxShapeType;
    Vec3                    m_vScale;
    Vec3                    m_vSpawnPos;

public:
    virtual void finaltick() override {};
    
public:
    void SetRigidbody(void* _pRigidbody);
    void SetShapeType(physx::PxGeometryType::Enum _ShapeInfo);
    void SetRigidScale(Vec3 _vScale) { m_vScale = _vScale; }
    void SetSpawnPos(Vec3 _pxSpawnPos) { m_vSpawnPos =  _pxSpawnPos; }
    void SetRigidPos(Vec3 _vPos);

    physx::PxRigidDynamic* GetRigidbody() { return m_PxRigidbody; }
    physx::PxGeometryType::Enum GetShapeType();
    Vec3 GetRigidScale() { return m_vScale; }
    Vec3 SetSpawnPos() { return m_vSpawnPos; }

    void AddForce(Vec3 _vForce);
    void ClearForce();

    void AddVelocity(Vec3 _vVelocity);
    void SetVelocity(Vec3 _vVelocity);
    void SetVelocityLimit(float _fLimit);
    void SetGravity(float _fGravity);
    Vec3 GetVelocity();
    void SetMass(float _fMass);

    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);

    CLONE(CRigidbody);

public:
    CRigidbody();
    ~CRigidbody();

    friend class CPhysXMgr;
};

