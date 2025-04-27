#include "pch.h"
#include "CStateScript.h"
#include "CUIMgr.h"
#include "CPlayerScript.h"
#include "CState.h"

CStateScript::CStateScript()
	: CScript((UINT)SCRIPT_TYPE::STATESCRIPT)
	, m_mapState{}
	, m_pCurState(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_tStat.HP, "HP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_tStat.Max_HP, "MaxHP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_tStat.MP, "MP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_tStat.Max_MP, "MaxMP");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_tStat.Speed, "Speed");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_tStat.Attack, "Attack");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_tStat.Attack_Speed, "Attack_Speed");
}

CStateScript::~CStateScript()
{
	Safe_Del_Map(m_mapState);
}

void CStateScript::begin()
{
}

void CStateScript::tick()
{
	if (nullptr == m_pCurState)
		return;

	m_pCurState->tick(); // 오브젝트의 현재 state tick을 돌려준다.
}

void CStateScript::SetStat(Stat _tStat)
{
	if (L"Player" == GetOwner()->GetName())
	{
		g_tPlayerStat = _tStat;
		CUIMgr::GetInst()->SetHP(_tStat.HP);
		CUIMgr::GetInst()->SetMP(_tStat.MP, GetOwner()->GetScript<CPlayerScript>()->GetUseMagic());
	}
	m_tStat = _tStat;
}
void CStateScript::AddState(const wstring& _strKey, CState* _pState)
{
	if (FindState(_strKey))
		return;

	m_mapState.insert(make_pair(_strKey, _pState));
	_pState->m_pOwnerScript = this;
}

void CStateScript::ChangeState(const wstring& _strStateName)
{
	// 인자로 들어온 이름으로 State를 찾아 다음 State로 지정함.
	CState* pNextState = FindState(_strStateName);
	assert(pNextState);

	// 이미 현재 State인 것으로 교체하려고 하면 아무일도 안일어나게 함.
	if (pNextState == m_pCurState)
		return;

	if (nullptr != m_pCurState)
		m_pCurState->Exit();

	m_pCurState = pNextState;

	m_pCurState->Enter();
}

void CStateScript::BeginOverlap(CCollider3D* _Other)
{
	if(nullptr !=m_pCurState)
		m_pCurState->BeginOverlap(_Other);
}

void CStateScript::OnOverlap(CCollider3D* _Other)
{
	if (nullptr != m_pCurState)
		m_pCurState->OnOverlap(_Other);
}

void CStateScript::EndOverlap(CCollider3D* _Other)
{
	if (nullptr != m_pCurState)
		m_pCurState->EndOverlap(_Other);
}

void CStateScript::SaveToLevelFile(FILE* _FILE)
{
	fwrite(&m_tStat, sizeof(Stat), 1, _FILE);
}

void CStateScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_tStat, sizeof(Stat), 1, _FILE);
}
