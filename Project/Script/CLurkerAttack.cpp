#include "pch.h"
#include "CLurkerAttack.h"
#include "CLurkerScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CSoundScript.h"

void CLurkerAttack::Enter()
{
	GetOwner()->Animator3D()->Play(5, false);

	// 공격 방향은 처음에만 지정해야 함. 
	m_Dir = GetOwner()->GetScript<CLurkerScript>()->GetMonsterToPlayerDir();
	GetOwner()->Rigidbody()->SetVelocityLimit(300.f);

	// 공격 직전에는 무조건 플레이어를 바라보도록. 
	GetOwner()->GetScript<CLurkerScript>()->SetStarePlayer(false);
	float fDir = GetDir(GetOwner()->Transform()->GetWorldPos(), GetOwner()->GetScript<CLurkerScript>()->GetPlayerPos());
	Vec3 CurDir = GetOwner()->Transform()->GetRelativeRot();
	GetOwner()->Transform()->SetRelativeRot(CurDir.x, fDir, 0.f);

	// Sound
	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(L"Sound\\Monster\\Lurker\\LurkerAttack1.ogg", 1, 0.1f);
}

void CLurkerAttack::tick()
{
	float AnimLength = (float)GetOwner()->Animator3D()->GetCurClipTimeLength();
	m_fTime += DT;
	float CurRatio = m_fTime / AnimLength;

	// 현재 재생중인 애니메이션이 어느정도 진행 된 경우 플레이어로 돌진. 그뒤로 속도가 다시 줄어들어서 제자리에 멈춰야 함.
	if (CurRatio >= 0.5f)
	{
		Vec3 Velocity = m_Dir;
		float fSpeed = GetOwnerScript()->GetStat().Speed;
		Velocity *= fSpeed * 30.f * DT;

		GetOwner()->Rigidbody()->AddVelocity(Velocity);

		// 공격 충돌체 프리펩
		CGameObject* MonsterAtack = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\MonsterAttack.prefab", (int)LAYER::MONSTERPROJECTILE, GetOwner()->Transform()->GetWorldPos(), 0.f);

		MonsterAtack->Collider3D()->SetOffsetPos(GetOwner()->Collider3D()->GetOffsetPos());
		MonsterAtack->Collider3D()->SetOffsetScale(GetOwner()->Collider3D()->GetOffsetScale());
	}

	// 애니메이션이 끝나면 Notify로 다시 변경.
	if (GetOwner()->Animator3D()->IsFinish())
		ChangeState(L"Notify");
}

void CLurkerAttack::Exit()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_fTime = 0.f;
}

CLurkerAttack::CLurkerAttack() :
	m_fTime(0.f)
{
}

CLurkerAttack::~CLurkerAttack()
{
}
