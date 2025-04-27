#include "pch.h"
#include "CMonsterScript.h"
#include "CStateScript.h"
#include "CSpawnMgr.h"


CMonsterScript::CMonsterScript(UINT SCRIPT_TYPE) :
	CScript(SCRIPT_TYPE)
	, m_pStateScript(nullptr)
	, m_pPlayer(nullptr)
	, m_bDetect(false)
	, m_bPaperBurnEffect(false)
	, m_bSendDeadTime(false)
	, m_fDeathTime(0.f)
	, m_fLastHitTime(-3.f)
	, m_bFixPos(false)
	, m_bSpawnByDoor(false)
{
}

CMonsterScript::~CMonsterScript()
{
	if (m_bSpawnByDoor)
		CSpawnMgr::GetInst()->ReduceMonsterCount();
}

void CMonsterScript::begin()
{
	// 플레이어 설정.
	if (nullptr == m_pPlayer)
		m_pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");

	// Crack Texture 바인딩.
	const int iMtrlCount = MeshRender()->GetMtrlCount();

	for (int i = 0; i < iMtrlCount; ++i)
	{
		Ptr<CMaterial> mtrl = MeshRender()->GetSharedMaterial(i);

		// 7번으로 보내자. CrackTexture.
		Ptr<CTexture> CrackTextue = CResMgr::GetInst()->Load<CTexture>(L"texture\\Deaths_Door\\MonsterCrack.png", L"texture\\Deaths_Door\\MonsterCrack.png");
		mtrl->SetTexParam(TEX_7, CrackTextue.Get());		

		// 6번으로 보내자. Paperburn용 noise texture.
		Ptr<CTexture> NoiseTextue = CResMgr::GetInst()->Load<CTexture>(L"texture\\Deaths_Door\\noise.png", L"texture\\Deaths_Door\\noise.png");
		mtrl->SetTexParam(TEX_6, NoiseTextue.Get());
	}
}

void CMonsterScript::tick()
{
	int iMtrlCount = MeshRender()->GetMtrlCount();
		
	for (int i = 0; i < iMtrlCount; ++i)
	{
		Ptr<CMaterial> mtrl = MeshRender()->GetDynamicMaterial(i);

		// 현재 체력 비율 보내기. FLOAT_0로 보냄.
		float HPRatio = (float)m_pStateScript->GetStat().HP / (float)m_pStateScript->GetStat().Max_HP;
		mtrl->SetScalarParam(FLOAT_0, &HPRatio);

		// Paperburn 효과를 주기 시작할 때, 현재까지 흐른 시간 보내기. FLOAT_1로 보냄. 한번만 보내야 해서m_bSendDeadTime를 사용함. 
		if (m_bPaperBurnEffect && !m_bSendDeadTime)
		{
			m_fDeathTime = GlobalData.tAccTime;
			m_bSendDeadTime = true;
		}
		
		if(m_bSendDeadTime)
			mtrl->SetScalarParam(FLOAT_1, &m_fDeathTime);

		// 피격 이펙트
		float fHitAfterTime = GlobalData.tAccTime - m_fLastHitTime;		// 피격 이후 지난 시간.
		int HitEffect = 0;

		if (fHitAfterTime < 0.2f && !m_bSendDeadTime)
			HitEffect = 1;

		mtrl->SetScalarParam(INT_3, &HitEffect);
	}

	if (m_bFixPos)
		Rigidbody()->SetRigidPos(m_vFixedPos);
}

void CMonsterScript::BeginOverlap(CCollider3D* _Other)
{
}

void CMonsterScript::OnOverlap(CCollider3D* _Other)
{
}

void CMonsterScript::EndOverlap(CCollider3D* _Other)
{
}

