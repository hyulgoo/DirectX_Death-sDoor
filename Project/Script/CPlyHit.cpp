#include "pch.h"
#include "CPlyHit.h"
#include "CPlayerScript.h"

CPlyHit::CPlyHit()
{
}

CPlyHit::~CPlyHit()
{
}

void CPlyHit::Enter()
{
	// Hit Anim 재생 끝나면 바로 Idle로 돌아가야 하므로 반복재생 false;
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::HIT, false);
	// 이미 Hit이므로 무적 활성화
	GetOwner()->GetScript<CPlayerScript>()->SetInvincible(true);
}

void CPlyHit::tick()
{
	GetOwner()->Rigidbody()->ClearForce();
	if(GetOwner()->Animator3D()->IsFinish())
	{
		if (GetOwner()->Animator3D()->GetCurClip() == (int)PLAYERANIM_TYPE::HIT)
		{
			GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::HIT_RECOVER, false);
		}
		else
		{
			GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
		}
	}
}

void CPlyHit::Exit()
{
	// 무적 비활성화
	GetOwner()->GetScript<CPlayerScript>()->SetInvincible(false);
	GetOwner()->Rigidbody()->ClearForce();
}

void CPlyHit::BeginOverlap(CCollider3D* _Other)
{
}

void CPlyHit::OnOverlap(CCollider3D* _Other)
{
}

void CPlyHit::EndOverlap(CCollider3D* _Other)
{
}
