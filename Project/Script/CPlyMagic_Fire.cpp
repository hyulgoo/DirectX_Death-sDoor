#include "pch.h"
#include "CPlyMagic_Fire.h"
#include <Engine/CDevice.h>
#include "CLevelSaveLoadInScript.h"
#include "CPlayerScript.h"
#include "CMagic_FireScript.h"
#include "CSoundScript.h"
#include "CUIMgr.h"

CPlyMagic_Fire::CPlyMagic_Fire()
	: m_vAttackDir{}
	, m_pFire(nullptr)
{
}

CPlyMagic_Fire::~CPlyMagic_Fire()
{
}

void CPlyMagic_Fire::Enter()
{
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::MAGIC_FIRE, false);
}

void CPlyMagic_Fire::tick()
{

	if (KEY_PRESSED(KEY::RBTN))
	{
		CalcDir();
	}
	if (GetOwner()->Animator3D()->IsFinish())
	{
		// 에너지가 부족하다면 Idle로 돌아가게 함.
		if (1 > GetOwnerScript()->GetStat().MP)
		{
			GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
		}
		else if (nullptr == m_pFire)
		{
			// Player 업그레이드 수치를 가져와 계수를 곱해 Bomb의 최종데미지를 정함.
			Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
			Vec3 vDir = GetOwner()->Transform()->GetXZDir();
			Vec3 vSpawnPos = Vec3(CurPos.x, CurPos.y + 100.f, CurPos.z) - vDir * 40.f;;
			m_pFire = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\Fire.prefab", (int)LAYER::PLAYERPROJECTILE, vSpawnPos);
			m_pFire->Transform()->SetRelativeScale(Vec3(0.45f));
			m_pFire->Transform()->SetRelativeRot(m_vAttackDir);
			m_pFire->Transform()->SetRelativePos(vSpawnPos);
		}
		else if (KEY_RELEASE(KEY::RBTN))
		{
			// 공격에 따른 에너지 소모
			Stat CurStat = GetOwnerScript()->GetStat();
			CurStat.MP -= 1;
			GetOwnerScript()->SetStat(CurStat);

			// Player 업그레이드 수치를 가져와 계수를 곱해 Fire의 최종데미지를 정함.
			float fDamage = GetOwnerScript()->GetStat().Spell_Power * (1.f + 0.3f * GetOwner()->GetScript<CPlayerScript>()->GetUpgrade(PLAYER_UPGRADE::MAGIC));
			fDamage *= 1.3f;
			Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
			Vec3 vDir = GetOwner()->Transform()->GetXZDir();
			Vec3 vSpawnPos = Vec3(CurPos.x, CurPos.y + 50.f, CurPos.z) - vDir * 20.f;
			m_pFire->Transform()->SetRelativeRot(m_vAttackDir);
			m_pFire->GetScript<CMagic_FireScript>()->SetDamege(fDamage);
			m_pFire->GetScript<CMagic_FireScript>()->SetDir(vDir);
			m_pFire->GetScript<CMagic_FireScript>()->SetStartPos(vSpawnPos);
			m_pFire->GetScript<CMagic_FireScript>()->SetCollidable();
			m_pFire->SetLifeSpan(2.f);

			GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");

			CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
			Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Player\\FireBallFire4.mp3", 1, 0.2f);
		}
		else
		{
			Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
			Vec3 vDir = GetOwner()->Transform()->GetXZDir();
			Vec3 vSpawnPos = Vec3(CurPos.x, CurPos.y + 40.f, CurPos.z) - vDir * 40.f;
			CalcDir();
			m_pFire->Transform()->SetRelativeRot(m_vAttackDir);
			m_pFire->Transform()->SetRelativePos(vSpawnPos);
		}
	}
	else
	{
		if (KEY_RELEASE(KEY::RBTN))
		{
			GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
		}
	}
}

void CPlyMagic_Fire::Exit()
{
	m_pFire = nullptr;
}

void CPlyMagic_Fire::CalcDir()
{
	Vec2 vCursorPos = CKeyMgr::GetInst()->GetMousePos();
	vCursorPos -= CDevice::GetInst()->GetRenderResolution() / 2.f;
	Vec3 vMousePos = Vec3(vCursorPos.x, 0.f, -vCursorPos.y);
	float fRot = GetDir(Vec3(0.f, 0.f, 0.f), vMousePos);
	GetOwner()->Transform()->SetRelativeRot(XM_PI * 1.5f, fRot, 0.f);
	m_vAttackDir = Vec3(XM_2PI - XM_PI * 1.5f, fRot, XM_2PI);
}