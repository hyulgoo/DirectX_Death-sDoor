#include "pch.h"
#include "CSlashScript.h"
#include "CStateScript.h"
#include "CMonsterScript.h"
#include "CLevelSaveLoadInScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>
#include "CGameCameraScript.h"

CSlashScript::CSlashScript()
	: CScript((UINT)SCRIPT_TYPE::SLASHSCRIPT)
{
}

CSlashScript::~CSlashScript()
{
}

void CSlashScript::BeginOverlap(CCollider3D* _Other)
{
	if(_Other->GetOwner()->GetScript<CStateScript>())
	{
		Stat CurStat = _Other->GetOwner()->GetScript<CStateScript>()->GetStat();
		CurStat.HP -= 50;
		if (CurStat.HP <= 0)
			CurStat.HP = 0;

		_Other->GetOwner()->GetScript<CStateScript>()->SetStat(CurStat);
		_Other->GetOwner()->GetScript<CMonsterScript>()->SetLastHitTime();

		CLevelSaveLoadInScript::SpawnPrefab(L"prefab\\HitEffect.prefab", (int)LAYER::DEFAULT, Transform()->GetWorldPos(), 0.2f);

		CAMERASHAKE(3.f, 800.f, 0.1f);

		Stat tCurPlyStat = CLevelMgr::GetInst()->FindObjectByName(L"Player")->GetScript<CStateScript>()->GetStat();
		tCurPlyStat.MP += 1;
		CLevelMgr::GetInst()->FindObjectByName(L"Player")->GetScript<CStateScript>()->SetStat(tCurPlyStat);
	}
}
