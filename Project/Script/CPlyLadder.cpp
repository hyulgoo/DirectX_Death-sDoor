#include "pch.h"
#include "CPlyLadder.h"
#include "CPlayerScript.h"

CPlyLadder::CPlyLadder()
	: m_fSpeed(200.f)
	, m_fStartYPos(0.f)
	, m_fLadderHeight(0.f)
	, m_fStartDelay(0.5f)
	, m_fGroundCheckDelay(0.f)
	, m_bEnd(false)
{
}

CPlyLadder::~CPlyLadder()
{
}

void CPlyLadder::Enter()
{
	m_fStartYPos = GetOwner()->Transform()->GetRelativePos().y;
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::LADDER_UP, true);

	// 사다리를 타는 중에는 무적
	GetOwner()->GetScript<CPlayerScript>()->SetInvincible(true);
}

void CPlyLadder::tick()
{
	if (m_fStartDelay > 0.f)
	{
		m_fStartDelay -= DT;
		return;
	}
		
	// 사다리 오르기가 끝났다면 LadderFinish Anim 재생 후 끝났다면 Idle로 전환
	if (m_bEnd)
	{
		if (GetOwner()->Animator3D()->GetCurClip() != (int)PLAYERANIM_TYPE::LADDER_FINISH)
		{
			GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::LADDER_FINISH, false);
			if (GetOwner()->Animator3D()->IsStop())
				GetOwner()->Animator3D()->SetStop(false);
			Vec3 Dir = GetOwner()->Transform()->GetXZDir();
			GetOwner()->Rigidbody()->SetVelocity(Dir * 30.f);
		}
		else if (GetOwner()->Animator3D()->IsFinish())
		{
			GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
		}

		return;
	}

	// 이동 중 Key가 Release됐다면 Anim Stop
	if (KEY_RELEASE(KEY::W) || KEY_RELEASE(KEY::S))
	{
		GetOwner()->Animator3D()->SetStop(true);
	}
	float Diff = GetOwner()->Transform()->GetRelativePos().y - m_fStartYPos;
	// 사다리 높이 이상으로 이동했다면 End로
	if (Diff >= m_fLadderHeight)
	{
		m_bEnd = true;
	}
	
	// 타는 도중 Dodge시 전환
	if (KEY_TAP(KEY::SPACE))
	{
		GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Dodge");
	}
	else
	{
		Move();
	}
}

void CPlyLadder::Exit()
{
	m_bEnd = false;
	m_fStartYPos = 0.f;
	m_fLadderHeight = 0.f;
	m_fStartDelay = 0.5f;
	m_fGroundCheckDelay = 0.f;
	GetOwner()->GetScript<CPlayerScript>()->SetInvincible(false);
}

void CPlyLadder::Move()
{
	// W, S를 누르고 있다면 위아래로 움직임. 이전에 멈춰있었다면 다시 애니메이션을 재생
	if (KEY_PRESSED(KEY::W))
	{
		GetOwner()->Rigidbody()->SetGravity(m_fSpeed);

		if (GetOwner()->Animator3D()->GetCurClip() != (int)PLAYERANIM_TYPE::LADDER_UP)
			GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::LADDER_UP, true);
		else
		{
			if (GetOwner()->Animator3D()->IsStop())
				GetOwner()->Animator3D()->SetStop(false);
		}
	}
	else if (KEY_PRESSED(KEY::S))
	{
		GetOwner()->Rigidbody()->SetGravity(-m_fSpeed);

		if (GetOwner()->Animator3D()->GetCurClip() != (int)PLAYERANIM_TYPE::LADDER_DOWN)
			GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::LADDER_DOWN, true);
		else
		{
			if (GetOwner()->Animator3D()->IsStop())
				GetOwner()->Animator3D()->SetStop(false);

			if (0.1f > abs((GetOwner()->Transform()->GetRelativePos() - GetOwner()->Transform()->GetPrevPos()).y))
			{
				m_fGroundCheckDelay += DT;
				if (m_fGroundCheckDelay > 0.3f)
				{
					GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
				}
			}
		}
	}
}
