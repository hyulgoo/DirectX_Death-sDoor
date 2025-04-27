#include "pch.h"
#include "CBatIdle.h"
#include "CBatScript.h"
CBatIdle::CBatIdle()	
	: m_fTime(0.f)
{
}

CBatIdle::~CBatIdle()
{
}

void CBatIdle::Enter()
{
	GetOwner()->Animator3D()->Play(0, true);
}

void CBatIdle::tick()
{
	m_fTime += DT;
		
	if (GetOwner()->GetScript<CMonsterScript>()->IsDetect())
	{
		ChangeState(L"Recognize");
	}
	else
	{
		Vec3 vVelocity = {};
		float Speed = GetOwnerScript()->GetStat().Speed;
		if (0.f <= m_fTime && m_fTime < 0.4f)
		{
			vVelocity = Vec3(Speed, 0.f, Speed);
		}
		else if (0.4f <= m_fTime && m_fTime < 0.8f)
		{
			vVelocity = Vec3(-Speed, 0.f, Speed);
		}
		else if (0.8f <= m_fTime && m_fTime < 1.2f)
		{
			vVelocity = Vec3(-Speed, 0.f, -Speed);
		}
		else if (1.2f <= m_fTime && m_fTime < 1.6f)
		{
			vVelocity = Vec3(Speed, 0.f, -Speed);
		}
		else if (1.6f <= m_fTime)
			m_fTime = 0.f;

		GetOwner()->Rigidbody()->SetVelocity(vVelocity);
		GetOwner()->Transform()->CalcDir();
	}
	
}

void CBatIdle::Exit()
{
	m_fTime = 0.f;
}
