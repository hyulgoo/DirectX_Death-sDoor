#include "pch.h"
#include "CPlyFall.h"
#include "CPlayerScript.h"

CPlyFall::CPlyFall()
{
}

CPlyFall::~CPlyFall()
{
}

void CPlyFall::Enter()
{
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::FALL, true);
}

void CPlyFall::tick()
{
	Vec3 Diff = GetOwner()->Transform()->GetWorldPos() - GetOwner()->Transform()->GetPrevPos();
	if (Diff.y > -0.2f )
		m_fTimeToIdle += DT;

	if(m_fTimeToIdle > 0.1f)
		GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
}

void CPlyFall::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
}

void CPlyFall::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == (int)LAYER::FALLAREA)
	{
		Stat CurStat = GetOwnerScript()->GetStat();
		CurStat.HP -= 1;
		GetOwnerScript()->SetStat(CurStat);
	}

}

void CPlyFall::OnOverlap(CCollider3D* _Other)
{
}

void CPlyFall::EndOverlap(CCollider3D* _Other)
{
}
