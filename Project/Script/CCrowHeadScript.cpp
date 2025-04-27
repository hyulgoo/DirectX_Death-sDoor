#include "pch.h"
#include "CCrowHeadScript.h"

#include <Engine/CDetourMgr.h>

void CCrowHeadScript::begin()
{
	// 애니메이션 재생.
	GetOwner()->Animator3D()->Play(0, true);

	// 플레이어 설정.
	if (nullptr == m_pPlayer)
	{
		m_pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	}
}

void CCrowHeadScript::tick()
{
	Vec3 PlayerPos = m_pPlayer->Transform()->GetWorldPos();
	float fPlayerDistance = GetDistance(PlayerPos, GetOwner()->Transform()->GetWorldPos());

	Vec3 Dir;
	Dir = PlayerPos - Transform()->GetWorldPos();
	Dir.x /= fPlayerDistance;
	Dir.y /= fPlayerDistance;
	Dir.z /= fPlayerDistance;
	
	Vec3 Velocity = Dir;
	float fSpeed = 100.f;
	Velocity *= fSpeed * 10.f * DT;

	GetOwner()->Rigidbody()->AddVelocity(Velocity);

	float fDir = GetSmoothDir(GetOwner(), m_pPlayer);
	Vec3 CurDir = GetOwner()->Transform()->GetRelativeRot();
	GetOwner()->Transform()->SetRelativeRot(CurDir.x, fDir, 0.f);
}

void CCrowHeadScript::BeginOverlap(CCollider3D* _Other)
{
	// PlayerProjectile Layer의 물체와 충돌한 경우.
	if (_Other->GetOwner()->GetLayerIndex() == 4)
	{
		// 반사를 구현한다면 반사돼야 함. 이 경우 일정 시간 이후 사라져야 함. 음.. 근데 반사를 안만들거면 일정 시간 이후 알아서 사라지게 해야겠네? 흠... 반사를 한다면, 반사 된 경우 일정 시간 이후 사라지게 하고.
	}
}

void CCrowHeadScript::OnOverlap(CCollider3D* _Other)
{
}

void CCrowHeadScript::EndOverlap(CCollider3D* _Other)
{
}

CCrowHeadScript::CCrowHeadScript()
	: CScript((UINT)SCRIPT_TYPE::CROWHEADSCRIPT)
	, m_pPlayer(nullptr)
	, m_fSpeed(500.f)
{
}

CCrowHeadScript::CCrowHeadScript(const CCrowHeadScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::CROWHEADSCRIPT)
	, m_pPlayer(_Other.m_pPlayer)
	, m_fSpeed(_Other.m_fSpeed)
{
}

CCrowHeadScript::~CCrowHeadScript()
{
}
