#include "pch.h"
#include "CGruntJumpAttack.h"
#include "CGruntScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CSoundScript.h"

void CGruntJumpAttack::Enter()
{
	GetOwner()->Animator3D()->Play(12, false);

	// 공격 방향은 처음에만 지정해야 함. 
	m_Dir = GetOwner()->GetScript<CGruntScript>()->GetMonsterToPlayerDir();
	GetOwner()->Rigidbody()->SetVelocityLimit(1000.f);

	float fDir = GetDir(GetOwner()->Transform()->GetWorldPos(), GetOwner()->GetScript<CGruntScript>()->GetPlayerPos());
	Vec3 CurDir = GetOwner()->Transform()->GetRelativeRot();
	GetOwner()->Transform()->SetRelativeRot(CurDir.x, fDir, 0.f);

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Grunt\\Grunt_JumpAttack1.ogg", 1, 0.1f);
}

void CGruntJumpAttack::tick()
{
	// 공격 충돌체 프리펩
	CGameObject* MonsterAtack = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\MonsterAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.f);

	MonsterAtack->Collider3D()->SetOffsetPos(GetOwner()->Collider3D()->GetOffsetPos());
	MonsterAtack->Collider3D()->SetOffsetScale(GetOwner()->Collider3D()->GetOffsetScale() * 1.2f);

	float AnimLength = (float)GetOwner()->Animator3D()->GetCurClipTimeLength();
	m_fTime += DT;
	float CurRatio = m_fTime / AnimLength;

	if (CurRatio <= 0.5f)
	{
		Vec3 Velocity = m_Dir;
		float fSpeed = GetOwnerScript()->GetStat().Speed;
		Velocity *= fSpeed * 60.f * DT;

		GetOwner()->Rigidbody()->AddVelocity(Velocity);
	}

	if(CurRatio > 0.65f)
		GetOwner()->Rigidbody()->ClearForce();

	// 애니메이션이 끝나면 Run2로 다시 변경.
	if (GetOwner()->Animator3D()->IsFinish())
		ChangeState(L"Run2");
}

void CGruntJumpAttack::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_fTime = 0.f;
}

CGruntJumpAttack::CGruntJumpAttack() :
	m_fTime(0.f)
{
}

CGruntJumpAttack::~CGruntJumpAttack()
{
}
