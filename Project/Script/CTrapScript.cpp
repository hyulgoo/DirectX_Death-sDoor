#include "pch.h"
#include "CTrapScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CPhysXMgr.h>
#include "CLevelSaveLoadInScript.h"
#include "CFenceScript.h"
#include "CSpawnMgr.h"
#include "CRoomScript.h"

CTrapScript::CTrapScript() :
	CScript(SCRIPT_TYPE::TRAPSCRIPT)
	, m_iTrapNum(-1)
{
}

CTrapScript::~CTrapScript()
{
}

void CTrapScript::begin()
{
}

void CTrapScript::tick()
{	
}

void CTrapScript::BeginOverlap(CCollider3D* _Other)
{
	GetOwner()->DeleteComponent(COMPONENT_TYPE::COLLIDER3D);

	CLevel* curLevel = CLevelMgr::GetInst()->GetCurLevel();
	if (_Other->GetOwner()->GetName() == L"Player")
	{
		if (curLevel->GetLevelType() == (int)LEVEL_TYPE::CASTLE_FIELD)
		{
			if (GetOwner()->GetName() == L"Trap1")
			{
				GetOwner()->GetScript<CRoomScript>()->SetRoomNum(1);
				GetOwner()->GetScript<CRoomScript>()->SetWaveCount(1);//최대 웨이브 수
				
				vector<SpawnInfo> wave0 = {};
				SpawnInfo info;
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(2344.f, 630.f, 3000.f);
				wave0.push_back(info);
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(2600.f, 630.f, 2600.f);
				wave0.push_back(info);
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(1549.f, 630.f, 3000.f);
				wave0.push_back(info);
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(1941.f, 630.f, 2442.f);
				wave0.push_back(info);

				//vector<SpawnInfo> wave1;
				//info.PrefabName = L"prefab\\Bat.prefab";
				//info.SpawnPos = Vec3(2299.f, 630.f, 3074.f);
				//wave1.push_back(info);
				//info.PrefabName = L"prefab\\Bat.prefab";
				//info.SpawnPos = Vec3(1889.f, 630.f, 2680.f);
				//wave1.push_back(info);
				//info.PrefabName = L"prefab\\Bat.prefab";
				//info.SpawnPos = Vec3(1574.f, 630.f, 3047.f);
				//wave1.push_back(info);
				//info.PrefabName = L"prefab\\Bat.prefab";
				//info.SpawnPos = Vec3(1940.f, 630.f, 3440.f);
				//wave1.push_back(info);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(0, wave0[0].PrefabName, wave0[0].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(0, wave0[1].PrefabName, wave0[1].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(0, wave0[2].PrefabName, wave0[2].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(0, wave0[3].PrefabName, wave0[3].SpawnPos);
				/*GetOwner()->GetScript<CRoomScript>()->AddWaveMst(1, wave1[0].PrefabName, wave1[0].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(1, wave1[1].PrefabName, wave1[1].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(1, wave1[2].PrefabName, wave1[2].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(1, wave1[3].PrefabName, wave1[3].SpawnPos);*/
				
				CSpawnMgr::GetInst()->RegisterRoom(1, GetOwner()->GetScript<CRoomScript>());
				CSpawnMgr::GetInst()->SpawnMonster(1);
				
				//trapped 되었으면 fence를 오픈하고 rigidbody PhysX설정해준다
				
				CGameObject* door1 = CLevelMgr::GetInst()->FindObjectByName(L"Fence1");
				door1->GetScript<CFenceScript>()->SetRoomNum(1);
				door1->MeshRender()->SetDynamicShadow(true);

				CGameObject* door2 = CLevelMgr::GetInst()->FindObjectByName(L"Fence2");
				door2->GetScript<CFenceScript>()->SetRoomNum(1);
				door2->MeshRender()->SetDynamicShadow(true);

				CGameObject* door3 = CLevelMgr::GetInst()->FindObjectByName(L"Fence3");
				door3->GetScript<CFenceScript>()->SetRoomNum(1);
				door3->MeshRender()->SetDynamicShadow(true);

				CSpawnMgr::GetInst()->RegisterFence(1, door1->GetScript<CFenceScript>());
				CSpawnMgr::GetInst()->RegisterFence(1, door2->GetScript<CFenceScript>());
				CSpawnMgr::GetInst()->RegisterFence(1, door3->GetScript<CFenceScript>());
				CSpawnMgr::GetInst()->ActivateFence(1, true);//연다					
			}
			else if (GetOwner()->GetName() == L"Trap2")
			{
				GetOwner()->GetScript<CRoomScript>()->SetRoomNum(2);
				GetOwner()->GetScript<CRoomScript>()->SetWaveCount(2);//최대 웨이브 수

				vector<SpawnInfo> wave0 = {};
				SpawnInfo info;
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(3760.f, 630.f, 719.f);
				wave0.push_back(info);
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(4064.f, 630.f, 719.f);
				wave0.push_back(info);
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(4004.f, 630.f, 961.f);
				wave0.push_back(info);

				vector<SpawnInfo> wave1;
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(3760.f, 630.f, 719.f);
				wave1.push_back(info);
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(4064.f, 630.f, 719.f);
				wave1.push_back(info);
				info.PrefabName = L"prefab\\Bat.prefab";
				info.SpawnPos = Vec3(4004.f, 630.f, 961.f);
				wave1.push_back(info);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(0, wave0[0].PrefabName, wave0[0].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(0, wave0[1].PrefabName, wave0[1].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(0, wave0[2].PrefabName, wave0[2].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(1, wave1[0].PrefabName, wave1[0].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(1, wave1[1].PrefabName, wave1[1].SpawnPos);
				GetOwner()->GetScript<CRoomScript>()->AddWaveMst(1, wave1[2].PrefabName, wave1[2].SpawnPos);

				CSpawnMgr::GetInst()->RegisterRoom(2, GetOwner()->GetScript<CRoomScript>());
				CSpawnMgr::GetInst()->SpawnMonster(2);

				//trapped 되었으면 fence를 오픈하고 rigidbody PhysX설정해준다

				CGameObject* door4 = CLevelMgr::GetInst()->FindObjectByName(L"Fence4");
				door4->GetScript<CFenceScript>()->SetRoomNum(2);
				door4->MeshRender()->SetDynamicShadow(true);

				CSpawnMgr::GetInst()->RegisterFence(2, door4->GetScript<CFenceScript>());
				CSpawnMgr::GetInst()->ActivateFence(2, true);//연다				
			}
		}
		else if (curLevel->GetLevelType() == (int)LEVEL_TYPE::FOREST_FIELD)
		{
			if (GetOwner()->GetName() == L"Trap1")
			{
				CGameObject* door = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\Fence.prefab", (int)LAYER::ITEM, Vec3(5942.f, 549.f, 3636.f));
				door->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
				Vec3 rot = (Vec3(270.f, 89.f, 0.f) / 180.f) * XM_PI;
				door->SetName(L"Fence");
				door->Transform()->SetRelativeRot(rot);
				door->AddComponent(new CRigidbody);
				door->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
				door->MeshRender()->SetDynamicShadow(true);
				//CPhysXMgr::GetInst()->CreateStaticCube(Vec3(5942.f, 549.f, 3636.f), Vec3(500.f, 500.f, 500.f), door);

				CGameObject* door1 = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\Fence.prefab", (int)LAYER::ITEM, Vec3(5811.f, 550.f, 6159.f));
				door1->Transform()->SetRelativeScale(Vec3(0.4f, 0.4f, 0.4f));
				Vec3 rot1 = (Vec3(270.f, 89.f, 0.f) / 180.f) * XM_PI;
				door1->SetName(L"Fence1");
				door1->Transform()->SetRelativeRot(rot1);
				door1->AddComponent(new CRigidbody);
				door1->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
				door1->MeshRender()->SetDynamicShadow(true);
				//CPhysXMgr::GetInst()->CreateStaticCube(Vec3(5811.f, 550.f, 6159.f), Vec3(200.f, 200.f, 200.f), door1);//till here							
			}
		}		
	}	
}
