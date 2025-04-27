#include "pch.h"
#include "CCrowBossBatBullet.h"
#include "CCrowBossScript.h"

void CCrowBossBatBullet::Enter()
{
	GetOwner()->Animator3D()->Play(6, true);
}

void CCrowBossBatBullet::tick()
{
}

void CCrowBossBatBullet::Exit()
{
}

CCrowBossBatBullet::CCrowBossBatBullet()
{
}

CCrowBossBatBullet::~CCrowBossBatBullet()
{
}
