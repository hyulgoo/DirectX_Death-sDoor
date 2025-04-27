#include "pch.h"
#include "CPlyDodge.h"
#include "CPlayerScript.h"
#include "CSoundScript.h"

CPlyDodge::CPlyDodge()
{
}

CPlyDodge::~CPlyDodge()
{
}

void CPlyDodge::Enter()
{
	float fSpeed = GetOwnerScript()->GetStat().Speed;
	Dodge(fSpeed);
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::DODGE, false);
	GetOwner()->GetScript<CPlayerScript>()->SetInvincible(true);

	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Player\\PlayerRoll.ogg", 1, 0.5f);
}

void CPlyDodge::tick()
{
	if (GetOwner()->Animator3D()->IsFinish())
		GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
}

void CPlyDodge::Exit()
{
	GetOwner()->GetScript<CPlayerScript>()->SetInvincible(false);
}

void CPlyDodge::Dodge(float _fSpeed)
{
	Vec3 vDir = Vec3(0.f, 0.f, 0.f);
	float fDir = 0.f;
	if(!(KEY_PRESSED(KEY::W)) && !(KEY_PRESSED(KEY::A)) && !(KEY_PRESSED(KEY::S)) && !(KEY_PRESSED(KEY::D)))
	{
		vDir = GetOwner()->Transform()->GetXZDir();
		fDir = GetOwner()->Transform()->GetRelativeRot().y;
	}
	else 
	{
		if (KEY_PRESSED(KEY::W))
		{
			vDir.z = 1.f;
		}
		if (KEY_PRESSED(KEY::S))
		{
			vDir.z = -1.f;
		}
		if (KEY_PRESSED(KEY::A))
		{
			vDir.x = -1.f;
		}
		if (KEY_PRESSED(KEY::D))
		{
			vDir.x = 1.f;
		}
		vDir.Normalize();
	}

	Vec2 vDefault = Vec2(0.f, -1.f);
	Vec2 v2Dir = Vec2(vDir.x, vDir.z);
	float angle = (float)acos(v2Dir.Dot(vDefault));

	if (vDir.x > 0.f)
		angle = XM_2PI - angle;

	GetOwner()->Rigidbody()->SetVelocity(vDir * _fSpeed * 1.5f);
	GetOwner()->Transform()->SetRelativeRot(XM_PI * 1.5f, angle, 0.f);
}
