#include "pch.h"
#include "CBankNPCScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CPlayerScript.h"

CBankNPCScript::CBankNPCScript()		:
	CScript(SCRIPT_TYPE::BANKNPCSCRIPT)
	, m_pTalkSign(nullptr)
{
}

CBankNPCScript::~CBankNPCScript()
{
}

void CBankNPCScript::begin()
{
	if (nullptr == m_pTalkSign)
	{
		m_pTalkSign = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\Talk.prefab", 31, Vec3(0.f, 0.f, 0.f));
		m_pTalkSign->MeshRender()->GetDynamicMaterial(0);
		m_pTalkSign->Transform()->SetRelativeScale(Vec3(0.f));
	}
}

void CBankNPCScript::tick()
{
}

void CBankNPCScript::BeginOverlap(CCollider3D* _Other)
{
	m_pTalkSign->Transform()->SetRelativeScale(81.f, 38.f, 1.f);		
}

void CBankNPCScript::OnOverlap(CCollider3D* _Other)
{
	if (L"Player" == _Other->GetOwner()->GetName())
	{
		
		if (KEY_TAP(KEY::E) && LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetCurLevel()->GetState())
		{
			// 상점 이용 시 Player의 이동을 막음.
			_Other->GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Idle");
			_Other->GetOwner()->GetScript<CPlayerScript>()->SetMoveAble(false);

			m_pTalkSign->Transform()->SetRelativeScale(Vec3(0.f));

			//상점 Frame & Upgrade 스텟 spawn
			
			if (!CLevelMgr::GetInst()->FindObjectByName(L"BankUIFrame"))
			{
				CGameObject* BankUIFrame = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\BankUIFrame.prefab", (int)LAYER::UI, Vec3(0.f, 0.f, 100.f));
			}
			if (!CLevelMgr::GetInst()->FindObjectByName(L"BankUIUpgrade"))
			{
				CGameObject* BankUIUpgrade = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\BankUIUpgrade.prefab", (int)LAYER::UI, Vec3(0.f));
			}

			//Main Cam 각도 변경
			CGameObject* mainCam = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
			mainCam->Transform()->SetRelativePos(Vec3(1698.f, 1443.f, 3470.f));
			Vec3 rot = (Vec3(8.f, -62.f, 0.f) / 180.f) * XM_PI;
			mainCam->Transform()->SetRelativeRot(rot);
		}
		if (KEY_TAP(KEY::ESC))
		{
			CGameObject* UIFrame = CLevelMgr::GetInst()->FindObjectByName(L"BankUIFrame");
			if(UIFrame)
				UIFrame->SetLifeSpan(0.f);
			CGameObject* UIUpgrade = CLevelMgr::GetInst()->FindObjectByName(L"BankUIUpgrade");
			if(UIUpgrade)
				UIUpgrade->SetLifeSpan(0.f);
			
			// Player의 이동제한 해제
			_Other->GetOwner()->GetScript<CPlayerScript>()->SetMoveAble(true);
		}
	}

}

void CBankNPCScript::EndOverlap(CCollider3D* _Other)
{
	m_pTalkSign->Transform()->SetRelativeScale(Vec3(0.f));
}
