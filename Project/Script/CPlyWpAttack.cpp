#include "pch.h"
#include "CPlyWpAttack.h"

CPlyWpAttack::CPlyWpAttack()
	: m_iAttackCount(0)
{
}

CPlyWpAttack::~CPlyWpAttack()
{
}

void CPlyWpAttack::Enter()
{
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::SLASH_R, false);
}

void CPlyWpAttack::tick()
{

}

void CPlyWpAttack::Exit()
{
	m_iAttackCount = 0;
}

void CPlyWpAttack::SetAttackCount(UINT _iAttackCount)
{ 
	m_iAttackCount = _iAttackCount; 
		if (1 == m_iAttackCount)
		GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::SLASH_L, false);
	else
		GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::SLASH_R, false);
}
