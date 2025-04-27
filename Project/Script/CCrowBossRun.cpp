#include "pch.h"
#include "CCrowBossRun.h"
#include "CCrowBossScript.h"
#include "CLevelSaveLoadInScript.h"

void CCrowBossRun::Enter()
{
	GetOwner()->Animator3D()->Play(14, false);
}

void CCrowBossRun::tick()
{
	// 공격 충돌체 프리펩
	CGameObject* MonsterAtack = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\MonsterAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.f);

	MonsterAtack->Collider3D()->SetOffsetPos(GetOwner()->Collider3D()->GetOffsetPos());
	MonsterAtack->Collider3D()->SetOffsetScale(GetOwner()->Collider3D()->GetOffsetScale());

	m_Dir = GetOwner()->GetScript<CCrowBossScript>()->GetMonsterToPlayerDir();

	Vec3 Velocity = m_Dir;
	float fSpeed = GetOwnerScript()->GetStat().Speed;
	Velocity *= fSpeed * 10.f * DT;

	GetOwner()->Rigidbody()->AddVelocity(Velocity);

	// 애니메이션이 끝난 경우.
	if (GetOwner()->Animator3D()->IsFinish())
	{
		++m_iTime;
		GetOwner()->Animator3D()->Play(14, false);

		// 애니메이션 4번 재생 후 다른 State로
		if (m_iTime>=3)
		{
			ChangeState(L"RightSpin");
		}
	}
}

void CCrowBossRun::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_iTime = 0;
}

CCrowBossRun::CCrowBossRun() :
	m_iTime(0)
{
}

CCrowBossRun::~CCrowBossRun()
{
}
