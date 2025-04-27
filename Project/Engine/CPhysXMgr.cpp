#include "pch.h"
#include "CPhysXMgr.h"
#include "components.h"
#include "CTimeMgr.h"
#include "CResMgr.h"

CPhysXMgr::CPhysXMgr()
    : m_vecDynamicObject{}
    , m_vecDynamicActor{}
    , m_vecStaticActor{}
{
}

CPhysXMgr::~CPhysXMgr()
{
    // Cleanup
    if(m_Dispatcher != nullptr)
        m_Dispatcher->release();
    if (m_Scene != nullptr)
    {
        m_Scene->release();
    }
    if (m_Material != nullptr)
        m_Material->release();
    if (m_Physics != nullptr)
        m_Physics->release();

    if (m_Pvd)
    {
        m_Pvd->release();
        m_Pvd = nullptr;
    }
    if (m_Cooking != nullptr)
        m_Cooking->release();
    if (m_Transport != nullptr)
        m_Transport->release();
    if (m_Foundation != nullptr)
        m_Foundation->release();
}

void CPhysXMgr::init()
{
    // Init PhysX
    // Foundation init (메모리 할당, 오류 보고, 디버그 출력 등의 기능을 제공)
    m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
    if (!m_Foundation)
        MessageBoxA(nullptr, "Error", "PxCreateFoundation failed!", MB_OK);

    // PhysX Visual Debugger init
    m_Pvd = PxCreatePvd(*m_Foundation);
    m_Transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    m_Pvd->connect(*m_Transport, physx::PxPvdInstrumentationFlag::eALL);

    m_ToleranceScale.length = 100;   // typical length of an object;
    m_ToleranceScale.speed = 981;    // typical speed of an object, gravity*1s is a reasonable choice;

    // Physics init (PhysX 주요 기능을 제공하는 객체로, 물리 장면, 물체, 물질 등을 생성하는 기능을 제공)
    m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd);
    if (!m_Physics)
        MessageBoxA(nullptr, "Error", "PxCreatePhysics failed!", MB_OK);

    // Physics 객체에서 쿠킹 객체 생성
    // Mesh를 가져오는 것을 cook이라고 함.
    physx::PxCookingParams mParam(m_ToleranceScale);
    mParam.meshWeldTolerance = 0.001f;
    mParam.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
    m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, mParam);

    // Create scene
    physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_Dispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    m_Scene = m_Physics->createScene(sceneDesc);
    m_Scene->setBounceThresholdVelocity(0.1f);
    physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    // Create material (물리 객체 표면의 마찰력, 반탄력 등 설정)
    m_Material = m_Physics->createMaterial(10.f, 5.f, 0.f);
}

void CPhysXMgr::tick()
{
    // Run simulation
   m_Scene->simulate(DT);
   if(m_Scene->fetchResults(true))
   {
       // 결과값을 가져와 Rigidbody를 사용하는 obj의 위치값에 적용시켜줌.
       // 현재 Scean의 중력이 매우 약하게 적용되므로 Velocity 300까지 중력 대신 아래로 밀어줌
       for (size_t i = 0; i < m_vecDynamicActor.size(); ++i)
       {
           if (nullptr == m_vecDynamicActor[i])
               continue;
           PxVec3 Velo = m_vecDynamicActor[i]->getLinearVelocity();

           if (Velo.y > -300.f)
               m_vecDynamicObject[i]->Rigidbody()->SetGravity(Velo.y - abs(Velo.y) * DT * 200.f);

           PxTransform GlobalPose = m_vecDynamicActor[i]->getGlobalPose();
           PxVec3 vVelocity = m_vecDynamicActor[i]->getLinearVelocity();
           m_vecDynamicObject[i]->Transform()->SetRelativePos(Vec3(GlobalPose.p.x, GlobalPose.p.y, GlobalPose.p.z));
       };
   }
}

void CPhysXMgr::finaltick()
{
    // 매 finaltick마다 가지고 있는 동적 물체들의 IsDead 여부를 확인하고 맞다면 Scean에서 삭제함.
    auto Objiter = m_vecDynamicObject.begin();
    auto Objiter_end = m_vecDynamicObject.end();
    for (; Objiter != Objiter_end;)
    {
        CGameObject* obj = *Objiter;
        if(obj->IsDead())
        {
            auto Dynamiciter = m_vecDynamicActor.begin();
            auto Dynamiciter_end = m_vecDynamicActor.end();
            for (; Dynamiciter != Dynamiciter_end; )
            {
                physx::PxRigidDynamic*  actor = *Dynamiciter;
                if (obj->Rigidbody()->GetRigidbody() == actor)
                {
                    Dynamiciter = m_vecDynamicActor.erase(Dynamiciter);
                    m_Scene->removeActor(*actor);
                    Dynamiciter_end = m_vecDynamicActor.end();          
                    continue;
                }
                ++Dynamiciter;
            }
            Objiter = m_vecDynamicObject.erase(Objiter);
            Objiter_end = m_vecDynamicObject.end();
            continue;
        }
        ++Objiter;
    }
}

physx::PxRigidDynamic* CPhysXMgr::CreateDynamic(Vec3 _vSpawnPos, const PxGeometry& _Geometry, CGameObject* _Object, float _fYOffset, const PxVec3& _Velocity)
{
    // 동적 물체 추가
    if (nullptr == _Object)
        assert(nullptr);
    const PxTransform& SpawnPos = PxTransform(_vSpawnPos.x, _vSpawnPos.y, _vSpawnPos.z);
    m_vecDynamicObject.push_back(_Object);
    physx::PxRigidDynamic* dynamic = PxCreateDynamic(*m_Physics, SpawnPos, _Geometry, *m_Material, 10.f);
    m_Scene->addActor(*dynamic);
    m_vecDynamicActor.push_back(dynamic);
    // Obj의 Rigidbody에 정보 세팅
    _Object->Rigidbody()->SetSpawnPos(_vSpawnPos);
    PxGeometryType::Enum type = _Geometry.getType();
    _Object->Rigidbody()->SetShapeType(_Geometry.getType());
    _Object->Rigidbody()->SetRigidbody(dynamic);
    return dynamic;
}

physx::PxRigidDynamic* CPhysXMgr::CreateCube(Vec3 _vSpawnPos, Vec3 _vCubeScale, CGameObject* _Object, Vec3 _vVelocity)
{
    const PxBoxGeometry& BoxGeometry = PxBoxGeometry(_vCubeScale.x, _vCubeScale.y, _vCubeScale.z);
    const PxVec3& Velocity = PxVec3(_vVelocity.x, _vVelocity.y, _vVelocity.z);
    _Object->Rigidbody()->SetRigidScale(_vCubeScale);
    return CreateDynamic(_vSpawnPos, BoxGeometry, _Object, 0);
}

physx::PxRigidDynamic* CPhysXMgr::CreateCapsule(Vec3 _vSpawnPos, float _fRadius, float _fHeight, CGameObject* _Object, Vec3 _vVelocity)
{
    const PxCapsuleGeometry& CapsuleGeometry = PxCapsuleGeometry(_fRadius, _fHeight);
    const PxVec3& Velocity = PxVec3(_vVelocity.x, _vVelocity.y, _vVelocity.z);
    Vec3 vCapsuleSclae = Vec3(_fRadius);
    vCapsuleSclae.y = _fHeight;
    _Object->Rigidbody()->SetRigidScale(vCapsuleSclae);
    return CreateDynamic(_vSpawnPos, CapsuleGeometry, _Object, 0);
}

physx::PxRigidDynamic* CPhysXMgr::CreateSphere(Vec3 _vSpawnPos, float _fRadius, CGameObject* _Object, Vec3 _vVelocity)
{
    const PxSphereGeometry& SphereGeometry = PxSphereGeometry(_fRadius);
    const PxVec3& Velocity = PxVec3(_vVelocity.x, _vVelocity.y, _vVelocity.z);
    _Object->Rigidbody()->SetRigidScale(Vec3(_fRadius));
    return CreateDynamic(_vSpawnPos, SphereGeometry, _Object, 0);
}

physx::PxRigidStatic* CPhysXMgr::ConvertStatic(Vec3 _vSpawnPos, CGameObject* _Object)
{
    const PxTransform& SpawnPos = PxTransform(_vSpawnPos.x, _vSpawnPos.y, _vSpawnPos.z);
    // Mesh의 정점 정보를 넣어준다
    Ptr<CMesh> pMesh = _Object->GetRenderComponent()->GetMesh();
    PxTriangleMeshDesc  meshDesc;
    meshDesc.points.count = pMesh->GetVtxCount();
    meshDesc.points.stride = sizeof(Vtx);
    meshDesc.points.data = pMesh->GetVtxSysMem();
    // Mesh의 Idx 버퍼 정보를 넣어준다
    meshDesc.triangles.count = pMesh->GetIdxInfo(0).iIdxCount / 3;;
    meshDesc.triangles.stride = 3 * sizeof(UINT);
    meshDesc.triangles.data = pMesh->GetIdxInfo(0).pIdxSysMem;

    // Physx에서는 Mesh를 생성하는 것을 Cooking이라고 함.
    // Mesh생성을 위한 write 버퍼 및 결과값을 받아내는 객체 생성
    PxDefaultMemoryOutputStream  writeBuffer;
    PxTriangleMeshCookingResult::Enum  result;
    bool  state = m_Cooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
    if (!state)
        assert(NULL);
    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

    // 받아온 Mesh 정보를 담은 버퍼로 정점포인트를 3개 사용하는 TriangleMesh를 생성
    PxTriangleMesh* tMesh = m_Physics->createTriangleMesh(readBuffer);
    PxTriangleMeshGeometry meshGeometry(tMesh);
    PxShape* meshShape = m_Physics->createShape(meshGeometry, *m_Material);

    // Mesh 정보와 Mtrl을 통해 전달받은 Mesh 모양 Shape을 생성해 Actor를 생성해 입혀줌.
    PxRigidStatic* pActor = m_Physics->createRigidStatic(SpawnPos);
    pActor->attachShape(*meshShape);
    pActor->setName(string(_Object->GetName().begin(), _Object->GetName().end()).c_str());
    // Scean에 Actor 등록
    m_Scene->addActor(*pActor);
    m_vecStaticActor.push_back(pActor);
    meshShape->release();
    tMesh->release();

    return pActor;
}

physx::PxRigidStatic* CPhysXMgr::CreateStaticCube(Vec3 _vSpawnPos, Vec3 _vCubeScale, CGameObject* _Object)
{
    // 정적 물체 추가
    if (nullptr == _Object)
        assert(nullptr);
    const PxBoxGeometry& BoxGeometry = PxBoxGeometry(_vCubeScale.x, _vCubeScale.y, _vCubeScale.z);

    const PxTransform& SpawnPos = PxTransform(_vSpawnPos.x, _vSpawnPos.y, _vSpawnPos.z);
    physx::PxRigidStatic* Static = PxCreateStatic(*m_Physics, SpawnPos, BoxGeometry, *m_Material);
    m_Scene->addActor(*Static);
    m_vecStaticActor.push_back(Static);

    return Static;
}

void CPhysXMgr::SetRigidPos(physx::PxRigidDynamic* _pDynamic, Vec3 _vPos)
{
    const PxTransform& Transform = PxTransform(_vPos.x, _vPos.y, _vPos.z);
    _pDynamic->setGlobalPose(Transform);
}

void CPhysXMgr::ReleaseStatic(physx::PxRigidStatic* _pStatic)
{
    // 중간에 삭제할 필요가 있는 Map 요소에 대해 삭제할 수 있는 정적 물체 삭제 기능

    vector< PxRigidStatic*>::iterator iter = m_vecStaticActor.begin();
    for (; iter != m_vecStaticActor.end(); ++iter)
    {
        PxRigidStatic* State = *iter;
        if (*iter == _pStatic)
        {
            m_vecStaticActor.erase(iter);
            m_Scene->removeActor(*_pStatic);
            return;
        }
    }
}

void CPhysXMgr::ReleaseDynamic(physx::PxRigidDynamic* _pDynamic, CGameObject* _pObject)
{
    if (m_vecDynamicObject.empty() && m_vecDynamicActor.empty())
        return;

    vector<CGameObject*>::iterator Objiter = m_vecDynamicObject.begin();
    vector<CGameObject*>::iterator Objiter_end = m_vecDynamicObject.end();
    int a = 0;
    for (; Objiter != Objiter_end; ++Objiter)
    {
        CGameObject* Obj = *Objiter;
        if (Obj == _pObject)
        {
            m_vecDynamicObject.erase(Objiter);
            a = 1;
        }
    }

    if (0 == a)
        assert(nullptr);

    vector<physx::PxRigidDynamic*>::iterator Dynamiciter = m_vecDynamicActor.begin();
    vector<physx::PxRigidDynamic*>::iterator Dynamiciter_end = m_vecDynamicActor.end();
    for (; Dynamiciter != Dynamiciter_end; ++Dynamiciter)
    {
        physx::PxRigidDynamic* Dynamic = *Dynamiciter;
        if (*Dynamiciter == _pDynamic)
        {
            m_vecDynamicActor.erase(Dynamiciter);
            m_Scene->removeActor(*_pDynamic);
            return;
        }
    }
    // 아무것도 삭제하지 못했다면 assert
    assert(nullptr);
}

PxRigidStatic* CPhysXMgr::CreatePlane(Vec4 _Plane)
{
    // 평면의 방정식을 이용한 전체 맵에 적용되는 평면 생성
    physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*m_Physics, physx::PxPlane(_Plane.x, _Plane.y, _Plane.z, _Plane.w), *m_Material);
    m_vecStaticActor.push_back(groundPlane);
    m_Scene->addActor(*groundPlane);
    return groundPlane;
}

void CPhysXMgr::AddDynamicActor(CRigidbody* _pRigidbody)
{
    m_vecDynamicObject.push_back(_pRigidbody->GetOwner());
    m_vecDynamicActor.push_back(_pRigidbody->m_PxRigidbody);
    m_Scene->addActor(*(_pRigidbody->m_PxRigidbody));
}

void CPhysXMgr::Clear()
{
    // Level 초기화에 호출될 전체 피직스 초기화   

    if(!m_vecDynamicActor.empty())
    {
        for (size_t i = 0; i < m_vecDynamicActor.size(); ++i)
        {
            if (nullptr != m_vecDynamicActor[i])
                m_Scene->removeActor(*m_vecDynamicActor[i]);
        }
    }

    if (!m_vecStaticActor.empty())
    {
        for (size_t i = 0; i < m_vecStaticActor.size(); ++i)
        {
            if (nullptr != m_vecStaticActor[i])
                m_Scene->removeActor(*m_vecStaticActor[i]);
        }
    }

    m_vecDynamicActor.clear();
    m_vecStaticActor.clear();
    m_vecDynamicObject.clear();
}

void CPhysXMgr::ChangeLevel(LEVEL_TYPE _tType)
{
    // 레벨 타입에 맞는 피직스 맵 로딩
    Ptr<CMeshData> pMeshData = nullptr; 
    switch (_tType)
    {
    case LEVEL_TYPE::CASTLE_FIELD:
        pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\PhysXmap\\Castle_Simple.fbx");
        break;
    case LEVEL_TYPE::CASTLE_BOSS:
        pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\PhysXmap\\Castle_Boss_Simple.fbx");
        break;
    case LEVEL_TYPE::FOREST_FIELD:
        pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\PhysXmap\\Forest_Simple.fbx");
        break;
    case LEVEL_TYPE::ICE_FIELD:
        pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\PhysXmap\\Ice_Simple.fbx");
        break;
    case LEVEL_TYPE::HALL:
        pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\PhysXmap\\Hall_Simple.fbx");
        break;
    case LEVEL_TYPE::ICE_BOSS:
        CreatePlane(Vec4(0.f, 1.f, 0.f, 0.f));
        break;
    case LEVEL_TYPE::LOADING:
    case LEVEL_TYPE::START:
        return;
    }

    // Ice_Boss Map은 fbx를 사용하지 않고 Plane을 깔기 때문에 MeshData 및 pMap을 건들 필요가 없음.
    if (LEVEL_TYPE::ICE_BOSS != _tType)
    {
        CGameObject* pMap = pMeshData->Instantiate();
        ConvertStatic(Vec3(0.f), pMap);
        delete pMap;
    }
}
