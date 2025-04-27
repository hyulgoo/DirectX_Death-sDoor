#include "pch.h"
#include "CCrowBossStandingDeath.h"
#include "CCrowBossScript.h"

void CCrowBossStandingDeath::Enter()
{
	GetOwner()->Animator3D()->Play(13, false);
}

void CCrowBossStandingDeath::tick()
{
	// 애니메이션이 끝난 경우 사망 Paperburn 효과 주기.
	if (GetOwner()->Animator3D()->IsFinish())
	{
		GetOwner()->GetScript<CCrowBossScript>()->SetPaperBurnEffect(true);
		m_bStartPaperBurn = true;
	}

	if (m_bStartPaperBurn)
		m_fPaperBurnTime += DT;

	// 지금까지 흐른 시간이 3초 이상이면 Destory.
	if (m_fPaperBurnTime > 3.f)
		GetOwnerScript()->Destroy();
}

void CCrowBossStandingDeath::Exit()
{
}

CCrowBossStandingDeath::CCrowBossStandingDeath() :
	m_bStartPaperBurn(false)
	, m_fPaperBurnTime(0.f)
{
}

CCrowBossStandingDeath::~CCrowBossStandingDeath()
{
}
