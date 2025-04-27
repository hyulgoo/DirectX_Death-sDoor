#include "pch.h"
#include "CPlyAttack.h"
#include "CPlayerScript.h"
#include "CPlyWpAttack.h"
#include "CLevelSaveLoadInScript.h"
#include "CSoundScript.h"

#include <Engine/CDevice.h>

CPlyAttack::CPlyAttack()
	: m_pSlash{}
	, m_iAttackCount(0)
	, m_vSlashPos{}
	, m_fRange(0.f)
	, m_fAcctime(0.f)
	, m_fDelay(0.f)
	, m_fAfterAttack(0.f)
	, m_fTimeToIdle(0.2f)
	, m_fAttackDir(0.f)
	, m_vMouseDir{}
	, m_fSlashStartTime(-3.f)
{
	m_pSlash[(UINT)SLASH::RIGHT] = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab//Slash_R.prefab", (int)LAYER::PLAYERPROJECTILE, Vec3(0.f, 0.f, 0.f));
	m_pSlash[(UINT)SLASH::LEFT] = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab//Slash_L.prefab", (int)LAYER::PLAYERPROJECTILE, Vec3(0.f, 0.f, 0.f));

	// Emissive로
	for (UINT i = 0; i < (UINT)SLASH::END; ++i)
	{
		int a = 1;
		m_pSlash[i]->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &a);
		m_pSlash[i]->MeshRender()->SetDynamicShadow(false);
		m_pSlash[i]->Transform()->SetRelativeScale(Vec3(0.f));
		m_pSlash[i]->Collider3D()->SetAbsolute(true);
		m_pSlash[i]->Collider3D()->SetOffsetScale(Vec3(0.f));
	}
}

CPlyAttack::~CPlyAttack()
{
}

void CPlyAttack::Enter()
{
	GetOwner()->Rigidbody()->ClearForce();
	m_fDelay = GetOwnerScript()->GetStat().Attack_Speed;
	m_fRange = 40.f + 4.f * GetOwner()->GetScript<CPlayerScript>()->GetUpgrade(PLAYER_UPGRADE::ATTACK);
	m_fSlashStartTime = GlobalData.tAccTime;
	CalcDir();
	m_vSlashPos = GetOwner()->Transform()->GetWorldPos() + Vec3(0.f, 20.f, 0.f) - m_vMouseDir * 80.f;
	Slash();
}

void CPlyAttack::tick()
{
	m_vSlashPos = GetOwner()->Transform()->GetWorldPos() + Vec3(0.f, 20.f, 0.f) - m_vMouseDir * 80.f;

	m_pSlash[(UINT)SLASH::RIGHT]->Transform()->SetRelativePos(Vec3(m_vSlashPos.x, m_vSlashPos.y - 30.f, m_vSlashPos.z));
	m_pSlash[(UINT)SLASH::LEFT]->Transform()->SetRelativePos(Vec3(m_vSlashPos.x, m_vSlashPos.y - 30.f, m_vSlashPos.z));

	m_fAcctime += DT;

	// 모션이 끝났거나 경과 시간이 공격 딜레이를 넘었다면 다음 공격으로 넘어감
	if (GetOwner()->Animator3D()->IsFinish() || m_fAcctime >= m_fDelay)
	{
		// 공격 모션이 끝나고 첫 번째 tick에만 현재 공격횟수가 증가하게 함
		if (0.f == m_fAfterAttack)
		{
			++m_iAttackCount;
			m_fSlashStartTime = GlobalData.tAccTime;
		}

		// Idle까지의 시간 증가
		m_fAfterAttack += DT;

		// 공격 3회가 끝났거나, 공격모션이 끝나도 추가 공격을 입력하지 않은 경우 Idle로 전환
		if (m_iAttackCount >= 3 || m_fAfterAttack >= m_fTimeToIdle)
		{
			GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
		}
		else if (KEY_PRESSED(KEY::LBTN))
		{
			CalcDir();

			// 무기에도 공격횟수를 알려줌.
			CPlyWpAttack* pWpAttackState = (CPlyWpAttack*)GetOwner()->GetChild()[0]->GetScript<CStateScript>()->FindState(L"Attack");
			pWpAttackState->SetAttackCount(m_iAttackCount);

			// 좌우 번갈아 공격함
			Slash();

			m_fAfterAttack = 0.f;
			m_fAcctime = 0.f;
		}
	}
	else if(m_fAcctime > m_fDelay * 0.8f)
	{
		SetSlashScale(false, SLASH::LEFT);
		SetSlashScale(false, SLASH::RIGHT);
		GetOwner()->Rigidbody()->ClearForce();

		if (KEY_TAP(KEY::SPACE))
			GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Dodge");
	}
	else
	{
		if (KEY_TAP(KEY::SPACE))
			GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Dodge");
	}
}

void CPlyAttack::Exit()
{
	m_iAttackCount = 0;
	m_fAcctime = 0.f;
	m_fAfterAttack = 0.f;
	m_vSlashPos = {};
	m_vMouseDir = {};
	SetSlashScale(false, SLASH::LEFT);
	SetSlashScale(false, SLASH::RIGHT);
}

void CPlyAttack::CalcDir()
{
	Vec2 vCursorPos = CKeyMgr::GetInst()->GetMousePos();
	vCursorPos -= CDevice::GetInst()->GetRenderResolution() / 2.f;
	Vec3 vMousePos = Vec3(vCursorPos.x, 0.f, -vCursorPos.y);
	m_fAttackDir = GetDir(Vec3(0.f, 0.f, 0.f), vMousePos);
	GetOwner()->Transform()->SetRelativeRot(XM_PI * 1.5f, m_fAttackDir, 0.f);

	m_vMouseDir = Vec3(0.f, 0.f, 0.f);
	m_vMouseDir = Vec3(0.f, 0.f, 0.f) - vMousePos;
	m_vMouseDir.Normalize();
	Vec3 AttackDir = m_vMouseDir * 50.f;
	AttackDir.y = 0.f;
	GetOwner()->Rigidbody()->SetVelocity(-AttackDir);
}

void CPlyAttack::Slash()
{
	m_pSlash[(UINT)SLASH::RIGHT]->Transform()->SetRelativePos(m_vSlashPos.x, m_vSlashPos.y - 30.f, m_vSlashPos.z);
	m_pSlash[(UINT)SLASH::LEFT]->Transform()->SetRelativePos(m_vSlashPos.x, m_vSlashPos.y - 30.f, m_vSlashPos.z);

	bool bRight = false;
	if (m_iAttackCount == 0 || m_iAttackCount % 2 == 0)
		bRight = true;

	int a = 1;
	Ptr<CTexture> NoiseTextue = CResMgr::GetInst()->Load<CTexture>(L"texture\\Deaths_Door\\noise.png", L"texture\\Deaths_Door\\noise.png");

	if (bRight)
	{
		GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::SLASH_R, false);
		SetSlashScale(true, SLASH::RIGHT);

		m_pSlash[(UINT)SLASH::RIGHT]->MeshRender()->GetMaterial(0)->SetScalarParam(INT_2, &a);
		m_pSlash[(UINT)SLASH::RIGHT]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_2, &m_fSlashStartTime);
		m_pSlash[(UINT)SLASH::RIGHT]->MeshRender()->GetMaterial(0)->SetTexParam(TEX_6, NoiseTextue.Get());
	}
	else
	{
		GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::SLASH_L, false);
		SetSlashScale(true, SLASH::LEFT);

		m_pSlash[(UINT)SLASH::LEFT]->MeshRender()->GetMaterial(0)->SetScalarParam(INT_2, &a);
		m_pSlash[(UINT)SLASH::LEFT]->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_2, &m_fSlashStartTime);
		m_pSlash[(UINT)SLASH::LEFT]->MeshRender()->GetMaterial(0)->SetTexParam(TEX_6, NoiseTextue.Get());
	}

	wstring wstrSoundFilePath;
	switch (m_iAttackCount)
	{
	case 0:
		wstrSoundFilePath = L"Sound\\Player\\SwordSwing1.ogg";
		break;
	case 1:
		wstrSoundFilePath = L"Sound\\Player\\SwordSwing2.ogg";
		break;
	case 2:
		wstrSoundFilePath = L"Sound\\Player\\SwordSwing3.ogg";
		break;
	}

	CSoundScript* soundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
	Ptr<CSound> pSound = soundscript->AddSound(wstrSoundFilePath, 1, 0.5f);
}

void CPlyAttack::SetSlashScale(bool _bOn, SLASH _tDir)
{
	if (_bOn)
	{
		switch (_tDir)
		{
		case SLASH::LEFT:
			m_pSlash[(UINT)SLASH::LEFT]->Transform()->SetRelativeScale(Vec3(-m_fRange));
			m_pSlash[(UINT)SLASH::LEFT]->Transform()->SetRelativeRot(XM_PI * (10.f / 18.f), m_fAttackDir + XM_2PI, 0.f);
			m_pSlash[(UINT)SLASH::LEFT]->Collider3D()->SetOffsetScale(Vec3(m_fRange * 5.2f));
			break;
		case SLASH::RIGHT:
			m_pSlash[(UINT)SLASH::RIGHT]->Transform()->SetRelativeScale(Vec3(m_fRange / 100.f));
			m_pSlash[(UINT)SLASH::RIGHT]->Transform()->SetRelativeRot(-XM_PI / 18.f, m_fAttackDir + XM_PI, 0.f);
			m_pSlash[(UINT)SLASH::RIGHT]->Collider3D()->SetOffsetScale(Vec3(m_fRange * 5.2f));
			break;
		}
	}
	else
	{
		switch (_tDir)
		{
		case SLASH::LEFT:
			m_pSlash[(UINT)SLASH::LEFT]->Transform()->SetRelativeScale(Vec3(0.f));
			m_pSlash[(UINT)SLASH::LEFT]->Collider3D()->SetOffsetScale(Vec3(0.f));
			break;
		case SLASH::RIGHT:
			m_pSlash[(UINT)SLASH::RIGHT]->Transform()->SetRelativeScale(Vec3(0.f));
			m_pSlash[(UINT)SLASH::RIGHT]->Collider3D()->SetOffsetScale(Vec3(0.f));
			break;
		}
	}
}
