#include "pch.h"
#include "CMagic_FireScript.h"
#include "CStateScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CSoundScript.h"
#include "CMonsterScript.h"

CMagic_FireScript::CMagic_FireScript()
	: CScript((UINT)SCRIPT_TYPE::MAGIC_FIRESCRIPT)
	, m_vStartPos{}
	, m_vDir{}
	, m_fDamage(0.f)
	, m_fSpeed(2000.f)
	, m_bCollidable(false)
{
}

CMagic_FireScript::~CMagic_FireScript()
{
}

void CMagic_FireScript::begin()
{
	Transform()->SetRelativePos(m_vStartPos);
	int a = 1;
	Vec4 Color = Vec4(1.f, 0.5f, 0.5f, 1.f);
	ParticleSystem()->GetMaterial(0)->SetScalarParam(INT_1, &a);
	ParticleSystem()->GetMaterial(0)->SetScalarParam(VEC4_0, &Color);
	ParticleSystem()->SetEmissive(true);
}

void CMagic_FireScript::tick()
{
	Vec3 CurPos = Transform()->GetRelativePos();
	CurPos += m_vDir * m_fSpeed * DT;
	Transform()->SetRelativePos(CurPos);
}

void CMagic_FireScript::BeginOverlap(CCollider3D* _Other)
{
	if (!m_bCollidable)
		return;
	if (_Other->GetOwner()->GetScript<CStateScript>())
	{
		if (_Other->GetOwner()->GetLayerIndex() == (int)LAYER::MONSTER)
		{
			Stat CurStat = _Other->GetOwner()->GetScript<CStateScript>()->GetStat();
			CurStat.HP -= m_fDamage;
			_Other->GetOwner()->GetScript<CStateScript>()->SetStat(CurStat);
			_Other->GetOwner()->GetScript<CMonsterScript>()->SetLastHitTime();
			Destroy();

			CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\HitEffect.prefab", (int)LAYER::DEFAULT, Transform()->GetRelativePos(), 0.2f);

			CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
			Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Player\\FireBallFire4.mp3", 1, 0.1f);
		}
	}

}

void CMagic_FireScript::EndOverlap(CCollider3D* _Other)
{
}

void CMagic_FireScript::SaveToLevelFile(FILE* _File)
{
}

void CMagic_FireScript::LoadFromLevelFile(FILE* _FILE)
{
}
