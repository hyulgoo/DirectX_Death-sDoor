#include "pch.h"
#include "CCrowBossSlidingReady.h"
#include "CCrowBossScript.h"
#include "CCrowBossSliding.h"
#include "CBossChainScript.h"
#include "CLevelSaveLoadInScript.h"

CCrowBossSlidingReady::CCrowBossSlidingReady()
	: m_vecHook{}
	, m_tChainPatern()
	, m_iThrowCount(0)
{
	// 시작 시 Hook 및 Chain 생성
	if (m_vecHook.empty())
	{
		for (int i = 0; i < 5; ++i)
		{
			CGameObject* Hook = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\BossHook.prefab", (int)LAYER::DEFAULT, Vec3(0.f, 0.f, 0.f));
			Hook->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
			Hook->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
			Hook->Collider3D()->SetAbsolute(true);
			Hook->Collider3D()->SetOffsetScale(Vec3(0.f));
			Hook->GetScript<CBossChainScript>()->SetChainScript(this);
			m_vecHook.push_back(Hook);

			vector<CGameObject*> vecChain;
			for (int j = 0; j < 70; ++j)
			{
				CGameObject* Chain = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\Chain.prefab", (int)LAYER::DEFAULT, Vec3(0.f, 0.f, 0.f));
				Chain->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
				vecChain.push_back(Chain);
				m_vecHook[i]->GetScript<CBossChainScript>()->SetChain(vecChain);
			}
		}
	}
}

CCrowBossSlidingReady::~CCrowBossSlidingReady()
{
	for (size_t i = 0; i < m_vecHook.size(); ++i)
	{
		if( nullptr != m_vecHook[i])
			m_vecHook[i]->SetLifeSpan(0.f);
	}
}

void CCrowBossSlidingReady::Enter()
{
	Vec3 vPlayerPos = CLevelMgr::GetInst()->FindObjectByName(L"Player")->Transform()->GetWorldPos();
	Vec3 vCurPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vDiff = vCurPos - vPlayerPos;

	GetOwner()->Animator3D()->Play(4, false);

	// 화면 안에 있다면 ONE 밖이라면 CROSS, SPREAD 중 랜덤지정
	if (vDiff.x > 700.f || vDiff.y > 400.f)
	{
		srand(unsigned int(time(0)));

		if(rand() % 2)
			m_tChainPatern = CHAINPATERN::CROSS;
		else
			m_tChainPatern = CHAINPATERN::SPREAD;

	}
	else
	{
		m_tChainPatern = CHAINPATERN::ONE;
	}

	// Hooking State에 HookObj 등록
	CCrowBossSliding* pSlidingState = (CCrowBossSliding*)GetOwnerScript()->FindState(L"Sliding");
	for(size_t i = 0; i < m_vecHook.size(); ++i)
	{
		pSlidingState->SetHook(m_vecHook);
		m_vecHook[i]->GetScript<CBossChainScript>()->SetSlidingScript(pSlidingState);
		if (CHAINPATERN::ONE != m_tChainPatern)
			pSlidingState->SetPatern(m_tChainPatern);
	}

	ThrowHook();
}

void CCrowBossSlidingReady::tick()
{
}

void CCrowBossSlidingReady::Exit()
{
	m_tChainPatern = {};
	m_iThrowCount = 0;
}

void CCrowBossSlidingReady::ThrowHook()
{
	Vec3 vPlayerPos = CLevelMgr::GetInst()->FindObjectByName(L"Player")->Transform()->GetRelativePos();
	if(CHAINPATERN::ONE == m_tChainPatern)
	{
		m_iThrowCount = 1;

		Vec3 vCurPos = GetOwner()->Transform()->GetWorldPos();
		Vec3 vDir = vPlayerPos - vCurPos;
		vDir.y = 0.f;
		vDir.Normalize();
		float YRot = GetDir(vCurPos, vPlayerPos);
		GetOwner()->Transform()->SetRelativeRot(Vec3(XM_PI * 1.5f, YRot, 0.f));
		m_vecHook[0]->Transform()->SetRelativePos(vCurPos);
		m_vecHook[0]->Transform()->SetRelativeRot(Vec3(0.f, YRot, 0.f));
		m_vecHook[0]->GetScript<CBossChainScript>()->SetThrowStartPos(vCurPos);
		m_vecHook[0]->GetScript<CBossChainScript>()->SetThrowDir(vDir);
		m_vecHook[0]->GetScript<CBossChainScript>()->SetThrowRot(Vec3(0.f, YRot, 0.f));
		m_vecHook[0]->GetScript<CBossChainScript>()->Active(true, false, 0.f);
		m_vecHook[0]->GetScript<CBossChainScript>()->SetThrowDistance(3500.f);
	}
	else if (CHAINPATERN::CROSS == m_tChainPatern)
	{
		m_iThrowCount = 2;

		vector<Vec3> vecThrowPos, vecThrowDir = {};
		Vec3 vHrzThrowPos = Vec3(vPlayerPos.x - 1750.f, vPlayerPos.y, vPlayerPos.z);
		Vec3 vHrzDir = vPlayerPos - vHrzThrowPos;
		vHrzDir.Normalize();
		Vec3 vVtThrowPos = Vec3(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z - 1750.f);
		Vec3 vVtDir = vPlayerPos - vVtThrowPos;
		vVtDir.Normalize();

		vecThrowPos.push_back(vHrzThrowPos);
		vecThrowDir.push_back(vHrzDir);

		vecThrowPos.push_back(vVtThrowPos);
		vecThrowDir.push_back(vVtDir);


		for (int i = 0; i < m_iThrowCount; ++i)
		{
			float YRot = 0;
			if (0 == i)
				YRot = GetDir(vHrzThrowPos, vPlayerPos);
			else
				YRot = GetDir(vVtThrowPos, vPlayerPos);

			m_vecHook[i]->GetScript<CBossChainScript>()->SetThrowStartPos(vecThrowPos[i]);
			m_vecHook[i]->GetScript<CBossChainScript>()->SetThrowDir(vecThrowDir[i]);
			m_vecHook[i]->GetScript<CBossChainScript>()->SetThrowRot(Vec3(0.f, YRot, 0.f));
			m_vecHook[i]->GetScript<CBossChainScript>()->SetThrowDistance(3500.f);
			m_vecHook[i]->GetScript<CBossChainScript>()->Active(true, true, 0.5f * i);
			if(0 == i)
				GetOwner()->Rigidbody()->SetRigidPos(vPlayerPos + Vec3(3000.f, 0.f, 3000.f));
		}
	}
	else if (CHAINPATERN::SPREAD == m_tChainPatern)
	{
		m_iThrowCount = 5;

		Vec3 vThrowDir;
		float X, Z;
		for (int i = 0; i < m_iThrowCount; ++i)
		{
			switch (i)
			{
			case 0:
				X = -300.f;
				Z = -1500.f; 
				vThrowDir = Vec3(0.5f, 0.f, 5.f).Normalize();
				break;
			case 1:
				X = 300.f;
				Z = 1500.f;
				vThrowDir = -Vec3(0.5f, 0.f, 5.f).Normalize();
				break;
			case 2:
				X = -600.f;
				Z = -1500.f;
				vThrowDir = Vec3(0.5f, 0.f, 5.f).Normalize();
				break;
			case 3:
				X = 600.f;
				Z = 1500.f;
				vThrowDir = -Vec3(0.5f, 0.f, 5.f).Normalize();
				break;
			case 4:
				X = -900.f;
				Z = -1500.f;
				vThrowDir = Vec3(0.5f, 0.f, 5.f).Normalize();
				break;
			}
			
			Vec3 vDiff = Vec3(X, 0.f, Z);
			Vec3 vThrowPos = vPlayerPos + vDiff;
			vThrowPos.y = 0;
			float YRot = GetDir(vThrowPos, vThrowPos + vThrowDir);
			if (0 == i)
				GetOwner()->Rigidbody()->SetRigidPos(vPlayerPos + Vec3(3000.f, 0.f, 3000.f));

			m_vecHook[i]->GetScript<CBossChainScript>()->SetThrowStartPos(vThrowPos);
			m_vecHook[i]->GetScript<CBossChainScript>()->SetThrowDir(vThrowDir);
			m_vecHook[i]->GetScript<CBossChainScript>()->SetThrowRot(Vec3(0.f, YRot, 0.f));
			m_vecHook[i]->GetScript<CBossChainScript>()->SetThrowDistance(3500.f);
			m_vecHook[i]->GetScript<CBossChainScript>()->Active(true, true, 0.5f * i);
		}
	}
}

void CCrowBossSlidingReady::HookArrive()
{
	--m_iThrowCount;
	if (0 >= m_iThrowCount)
	{
		ChangeState(L"Sliding");
	}
}
