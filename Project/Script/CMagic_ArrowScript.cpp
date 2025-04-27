#include "pch.h"
#include "CMagic_ArrowScript.h"
#include "CStateScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CSoundScript.h"
#include "CMonsterScript.h"

CMagic_ArrowScript::CMagic_ArrowScript()
	: CScript((UINT)SCRIPT_TYPE::MAGIC_ARROWSCRIPT)
	, m_vStartPos{}
	, m_vDir{}
	, m_fSpeed(2000.f)
	, m_fDamage(0.f)
	, m_bCollidable(false)
{
}

CMagic_ArrowScript::~CMagic_ArrowScript()
{
}
void CMagic_ArrowScript::begin()
{
	Transform()->SetRelativePos(m_vStartPos);
	int a = 1;
	Vec4 Color = Vec4(1.f, 0.f, 0.f, 1.f);
	MeshRender()->GetMaterial(0)->SetScalarParam(INT_1, &a);
	MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_0, &Color);
}

void CMagic_ArrowScript::tick()
{
	Vec3 CurPos = Transform()->GetRelativePos();
	CurPos += m_vDir * m_fSpeed * DT;
	Transform()->SetRelativePos(CurPos);
}

void CMagic_ArrowScript::BeginOverlap(CCollider3D* _Other)
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

			CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\HitEffect.prefab", (int)LAYER::DEFAULT, Transform()->GetRelativePos() + m_vDir * 40.f, 0.2f);

			CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
			Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Player\\ArrowHitEnemy.ogg", 1, 0.1f);
		}
	}
	 // 터지는 효과 등 나오게 할 것
}

void CMagic_ArrowScript::EndOverlap(CCollider3D* _Other)
{
}


void CMagic_ArrowScript::SaveToLevelFile(FILE* _File)
{
}

void CMagic_ArrowScript::LoadFromLevelFile(FILE* _FILE)
{
}

