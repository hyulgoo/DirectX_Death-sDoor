#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>

#include "CStateScript.h"
#include "PlayerStates.h"
#include "CPlayerWeaponScript.h"
#include "CLadderScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CGameCameraScript.h"

#include <Engine\CRenderMgr.h>
#include <Engine/CPhysXMgr.h>
#include <Engine/CEventMgr.h>
#include "CUIMgr.h"
#include <Engine/CDetourMgr.h>

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pStateScript(nullptr)
	, m_pSword(nullptr)
	, m_iCurMagic(0)
	, m_imoney(0)
	, m_arrUpgrade{}
	, m_fFallCheckTime(0.f)
	, m_bInvincible(false)
	, m_bEditorMode(true)
	, m_bDisableMove(false)
{
}

CPlayerScript::~CPlayerScript()
{
	if (nullptr == m_pStateScript)
		delete m_pStateScript;
}

void CPlayerScript::begin()
{
	CDetourMgr::GetInst()->RegisterPlayer(GetOwner());
	if (nullptr == m_pSword)
	{
		m_pSword = GetOwner()->GetChild()[0]->GetScript<CPlayerWeaponScript>();
	}
	if(nullptr == m_pStateScript)
	{
		m_pStateScript = GetOwner()->GetScript<CStateScript>(); 
		m_pStateScript->AddState(L"Idle", new CPlyIdle);
		m_pStateScript->AddState(L"Walk", new CPlyWalk);
		m_pStateScript->AddState(L"Run", new CPlyRun);
		m_pStateScript->AddState(L"Dodge", new CPlyDodge);
		m_pStateScript->AddState(L"Dance", new CPlyDance);
		m_pStateScript->AddState(L"Fall", new CPlyFall);
		m_pStateScript->AddState(L"Hit", new CPlyHit);
		m_pStateScript->AddState(L"Dead", new CPlyDead);
		m_pStateScript->AddState(L"Attack", new CPlyAttack);
		m_pStateScript->AddState(L"Ladder", new CPlyLadder);
		m_pStateScript->AddState(L"Arrow", new CPlyMagic_Arrow);
		m_pStateScript->AddState(L"Fire", new CPlyMagic_Fire);
		m_pStateScript->AddState(L"Bomb", new CPlyMagic_Bomb);
		m_pStateScript->AddState(L"Hook", new CPlyMagic_Hook);
		m_pStateScript->AddState(L"Hooking", new CPlyMagic_Hooking);
		m_pStateScript->SetStat(g_tPlayerStat);
	}

	if (!CLevelMgr::GetInst()->FindObjectByName(L"Cursor"))
	{
		CGameObject* pCursor = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\Cursor.prefab", (int)LAYER::UI, Vec3(0.f));
		pCursor->SetName(L"Cursor");
	}
	m_pStateScript->ChangeState(L"Idle");

	int tCurLevelType = CLevelMgr::GetInst()->GetCurLevel()->GetLevelType();
	Vec3 vStartPos = {};
	switch (LEVEL_TYPE(tCurLevelType))
	{
	case LEVEL_TYPE::CASTLE_FIELD:
		vStartPos = { 2020.f, 550.f, 664.f };
		break;
	case LEVEL_TYPE::CASTLE_BOSS:
		vStartPos = { 1655.f, 950.f, 1400.f };
		break;
	case LEVEL_TYPE::FOREST_FIELD:
		vStartPos = { 1000.f, 500.f, 350.f };
		break;
	case LEVEL_TYPE::ICE_FIELD:
		vStartPos = { 4451.f, 570.f, 661.f };
		break;
	case LEVEL_TYPE::ICE_BOSS:
		vStartPos = { 0.f, 0.f, 0.f };
		break;
	case LEVEL_TYPE::HALL:
		vStartPos = { 1164.f, 450.f, 392.f };
		break;
	}
	Rigidbody()->SetRigidPos(vStartPos);

	// Sword(Child0)과 Bow(Child1)에 Emissive효과 부여
	int a = 1;
	GetOwner()->GetChild()[0]->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &a);
	GetOwner()->GetChild()[1]->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &a);

	CUIMgr::GetInst()->ActiveMagic(m_iCurMagic);
}

void CPlayerScript::tick()
{
	// 숫자 1~4로 우클릭으로 사용하는 마법 효과 변경
	SetMagicType();

	// Hp, Mana 회복, 무적모드
	EditorMode();

	// Fall 상태 체크
	FallCheck();
}

void CPlayerScript::BeginOverlap(CCollider3D* _Other)
{
	if (m_bEditorMode)
		return;
	// 아래는 피격 관련으로 무적이라면 return;
	if (m_bInvincible)
		return;

	if ((int)LAYER::MONSTERPROJECTILE == _Other->GetOwner()->GetLayerIndex())
	{
		ChangeState(L"Hit");
		Stat CurStat = m_pStateScript->GetStat();
		CurStat.HP -= 1;
		m_pStateScript->SetStat(CurStat);
		// 맞았을 때 Hp가 0 이하라면 Dead 아니라면 Hit 상태로
		if (CurStat.HP <= 0)
			ChangeState(L"Dead");
		else
			ChangeState(L"Hit");

		// 맞을 때는 Hit 이펙트를 출력함.
		CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\HitEffect.prefab", (int)LAYER::DEFAULT, Transform()->GetRelativePos(), 0.5f);
		CAMERASHAKE(3.f, 800.f, 0.1f);
	}
}

void CPlayerScript::OnOverlap(CCollider3D* _Other)
{
	if ((int)LAYER::LADDER == _Other->GetOwner()->GetLayerIndex())
	{
		if (KEY_TAP(KEY::E))
		{	
			// 사다리가 바라보고 있는 방향, 위치로 플레이어를 고정시킴
			Vec3 vLadderRot = _Other->GetOwner()->Transform()->GetRelativeRot();
			GetOwner()->Transform()->SetRelativeRot(Vec3(XM_PI * 1.5f, vLadderRot.y, 0.f));
			Vec3 vLadderPos = _Other->GetOwner()->Transform()->GetRelativePos();
			GetOwner()->Rigidbody()->SetRigidPos(vLadderPos);

			ChangeState(L"Ladder");
			CPlyLadder* pLadderState =  (CPlyLadder*)m_pStateScript->FindState(L"Ladder");
			pLadderState->SetHeight(_Other->GetOwner()->GetScript<CLadderScript>()->GetHeight());
		}
	}
}

void CPlayerScript::EndOverlap(CCollider3D* _Other)
{
}

void CPlayerScript::ChangeState(wstring _strStateName)
{
	m_pStateScript->ChangeState(_strStateName);
	m_pSword->ChangeState(_strStateName);
}

void CPlayerScript::SetMagicType()
{
	if (KEY_TAP(KEY::_1))
	{
		m_iCurMagic = (UINT)PLAYER_MAGIC::ARROW;
		CUIMgr::GetInst()->ActiveMagic((int)PLAYER_MAGIC::ARROW);
	}
	else if (KEY_TAP(KEY::_2))
	{
		m_iCurMagic = (UINT)PLAYER_MAGIC::FIRE;
		CUIMgr::GetInst()->ActiveMagic((int)PLAYER_MAGIC::FIRE);
	}
	else if (KEY_TAP(KEY::_3))
	{
		m_iCurMagic = (UINT)PLAYER_MAGIC::BOMB;
		CUIMgr::GetInst()->ActiveMagic((int)PLAYER_MAGIC::BOMB);
	}
	else if (KEY_TAP(KEY::_4))
	{
		m_iCurMagic = (UINT)PLAYER_MAGIC::HOOK;
		CUIMgr::GetInst()->ActiveMagic((int)PLAYER_MAGIC::HOOK);
	}
}

void CPlayerScript::ChangeMagicState()
{
	switch (PLAYER_MAGIC(m_iCurMagic))
	{
	case PLAYER_MAGIC::ARROW:
		ChangeState(L"Arrow");
		break;
	case PLAYER_MAGIC::FIRE:
		ChangeState(L"Fire");
		break;
	case PLAYER_MAGIC::BOMB:
		ChangeState(L"Bomb");
		break;
	case PLAYER_MAGIC::HOOK:
		ChangeState(L"Hook");
		break;
	}
}

void CPlayerScript::FallCheck()
{
	if (m_pStateScript->GetCurState() != m_pStateScript->FindState(L"Dead")
		&& m_pStateScript->GetCurState() != m_pStateScript->FindState(L"Hit")
		&& m_pStateScript->GetCurState() != m_pStateScript->FindState(L"Ladder"))
	{
		if ((Transform()->GetPrevPos().y - Transform()->GetRelativePos().y) > 70.f * DT)
		{
			m_fFallCheckTime += DT;
			if (m_fFallCheckTime > 0.15f)
			{
				ChangeState(L"Fall");
				m_fFallCheckTime = 0.f;
			}
		}
	}
}

void CPlayerScript::EditorMode()
{
	if (KEY_TAP(KEY::Q))
	{
		Stat CurStat = m_pStateScript->GetStat();
		CurStat.MP = CurStat.Max_MP;
		m_pStateScript->SetStat(CurStat);
	}
	if (KEY_TAP(KEY::R))
	{
		Stat CurStat = m_pStateScript->GetStat();
		CurStat.HP = CurStat.Max_HP;
		m_pStateScript->SetStat(CurStat);
	}
	if (KEY_TAP(KEY::F))
	{
		m_bEditorMode = m_bEditorMode ? false : true;
	}
	// 끼임 탈출
	if (KEY_TAP(KEY::T))
	{
		// 사망 텍스쳐 출력 시간이 끝나면 현재레벨을 다시 시작함.
		int iCurLevelType = CLevelMgr::GetInst()->GetCurLevel()->GetLevelType();
		g_tNextLevel = (LEVEL_TYPE)iCurLevelType;
		CLevel* NewLevel = CLevelSaveLoadInScript::Stop(L"Level\\LLL.lv", LEVEL_STATE::STOP);
		NewLevel->SetName(L"LevelLoading");
		NewLevel->SetLevelType((int)LEVEL_TYPE::LOADING);
		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)NewLevel;
		evn.lParam = (DWORD_PTR)NewLevel->GetLevelType();
		CEventMgr::GetInst()->AddEvent(evn);
	}
}

void CPlayerScript::Upgrade(PLAYER_UPGRADE _Type)
{
	++m_arrUpgrade[(UINT)_Type];

	Stat CurStat = m_pStateScript->GetStat();
	switch (_Type)
	{
		// 무기 공격력 및 공격범위 증가
		// 20%씩 증가함
	case PLAYER_UPGRADE::ATTACK:
		CurStat.Attack *= 1.f + (0.2f * m_arrUpgrade[(UINT)PLAYER_UPGRADE::ATTACK]);
		break;
		// 공격딜레이 레발 당 10% 감소
	case PLAYER_UPGRADE::ATK_SPEED:
		CurStat.Attack_Speed *= 1.f - (0.1f * m_arrUpgrade[(UINT)PLAYER_UPGRADE::ATK_SPEED]);
		break;
		// 이동속도 레벨 당 10% 증가
	case PLAYER_UPGRADE::SPEED:
		CurStat.Speed *= 1.f + (0.1f * m_arrUpgrade[(UINT)PLAYER_UPGRADE::SPEED]);
		break;
		// 마법 공격력을 레벨 당 30%씩 증가
	case PLAYER_UPGRADE::MAGIC:
		CurStat.Spell_Power *= 1.f + (0.3f * m_arrUpgrade[(UINT)PLAYER_UPGRADE::MAGIC]);
		break;
	}
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iCurMagic, sizeof(UINT), 1, _File);
	fwrite(&m_imoney, sizeof(UINT), 1, _File);
	for (UINT i = 0; i < (UINT)PLAYER_UPGRADE::END; ++i)
		fwrite(&m_arrUpgrade[i], sizeof(UINT), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_iCurMagic, sizeof(UINT), 1, _File);
	fread(&m_imoney, sizeof(UINT), 1, _File);
	for (UINT i = 0; i < (UINT)PLAYER_UPGRADE::END; ++i)
		fread(&m_arrUpgrade[i], sizeof(UINT), 1, _File);
}