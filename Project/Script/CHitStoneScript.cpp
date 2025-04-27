#include "pch.h"
#include "CHitStoneScript.h"
#include "CSpawnMgr.h"
#include <Engine/CDetourMgr.h>

CHitStoneScript::CHitStoneScript() 
	: CScript(SCRIPT_TYPE::HITSTONESCRIPT)
	, m_bHit(false)
	, m_bMove(false)
	, m_fMoveDistance(0.f)
	, m_vStartPos{}
{
}

CHitStoneScript::~CHitStoneScript()
{
}

void CHitStoneScript::begin()
{
	CDetourMgr::GetInst()->ChangeLevel(LEVEL_TYPE::FOREST_FIELD);
}

void CHitStoneScript::tick()
{
	Vec3 vCurPos = Transform()->GetWorldPos();
	if (m_bHit && !m_bMove)
	{
		Transform()->SetRelativePos(vCurPos.x, vCurPos.y - 100.f * DT, vCurPos.z);
	}
	m_fMoveDistance = (m_vStartPos - Transform()->GetWorldPos()).Length();
	if (m_fMoveDistance > 100.f && m_bHit)
	{
		m_bMove = true;
	}
}

void CHitStoneScript::BeginOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == (int)LAYER::PLAYERPROJECTILE)
	{
		m_bHit = true;
		m_vStartPos = Transform()->GetWorldPos();
		GetOwner()->DeleteComponent(COMPONENT_TYPE::COLLIDER3D);
		CSpawnMgr::GetInst()->ActivateFence(1, true);
	}
}

void CHitStoneScript::OnOverlap(CCollider3D* _Other)
{
}

void CHitStoneScript::EndOverlap(CCollider3D* _Other)
{
}
