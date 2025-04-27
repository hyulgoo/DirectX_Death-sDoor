#include "pch.h"
#include "CRoomScript.h"
#include "CSpawnDoorScript.h"
#include "CSpawnMgr.h"
#include "CLevelSaveLoadInScript.h"
#include "CFenceScript.h"

CRoomScript::CRoomScript()
	: CScript((UINT)SCRIPT_TYPE::ROOMSCRIPT)
	, m_iRoomNum(-1)
	, m_iRemainMst(0)
	, m_iRemainGimmik(0)
	, m_iCurWaveNum(0)
	, m_iMaxWaveCount(0)
	, m_vecWave{}
	, m_bActive(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
}

CRoomScript::CRoomScript(const CRoomScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::ROOMSCRIPT)
	, m_iRoomNum(_Other.m_iRoomNum)
	, m_iRemainMst(-1)
	, m_iRemainGimmik(-1)
	, m_iCurWaveNum(0)
	, m_iMaxWaveCount(_Other.m_iMaxWaveCount)
	, m_vecWave()
	, m_bActive(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
}

CRoomScript::~CRoomScript()
{
	for (size_t i = 0; i < m_vecWave->size(); ++i)
	{
		m_vecWave[i].clear();
	}
}

void CRoomScript::begin()
{
	CSpawnMgr::GetInst()->RegisterRoom(m_iRoomNum, this);
}

void CRoomScript::tick()
{
}

void CRoomScript::SpawnMst()
{
	//현재 웨이브의 몬스터 수를 확인해서 스폰하고 m_iRemainMst에 수 표시
	for (size_t i = 0; i < m_vecWave[m_iCurWaveNum].size(); ++i)
	{
		CGameObject* pSpawnDoor = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\DoorPink.prefab", 8, m_vecWave[m_iCurWaveNum][i].SpawnPos);
		pSpawnDoor->AddComponent(new CSpawnDoorScript);
		pSpawnDoor->GetScript<CSpawnDoorScript>()->SetSpawnMst(m_vecWave[m_iCurWaveNum][i].PrefabName);
		pSpawnDoor->GetScript<CSpawnDoorScript>()->SetDelay(1.f);
		int a = 1;
		pSpawnDoor->MeshRender()->GetMaterial(0)->SetScalarParam(INT_1, &a);
		pSpawnDoor->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_0, Vec4(0.6f,0.1f,0.2f,0.7f));
		++m_iRemainMst;
	}
	++m_iCurWaveNum;
}

void CRoomScript::SetWaveInfo(int _iWaveNum, vector<SpawnInfo> _mapInfo)
{
	m_vecWave[_iWaveNum] = _mapInfo;
}

void CRoomScript::ReduceMonsterCount()
{
	if(m_iRemainMst > 0)
	{
		--m_iRemainMst;
		if (0 == m_iRemainMst)
		{
			if (m_iCurWaveNum < m_iMaxWaveCount)
				SpawnMst();
		}
	}


	if (m_iRemainMst <= 0 && m_iRemainGimmik <= 0
		&& m_iCurWaveNum == m_iMaxWaveCount)
	{
		CSpawnMgr::GetInst()->ActivateFence(m_iRoomNum, true);
	}
}

void CRoomScript::ReduceGimmickCount()
{
	if(m_iRemainGimmik > 0)
		--m_iRemainGimmik;
	if (m_iRemainMst <= 0 && m_iRemainGimmik <= 0
		&& m_iCurWaveNum == m_iMaxWaveCount)
	{
		CSpawnMgr::GetInst()->ActivateFence(m_iRoomNum, true);
	}
}

void CRoomScript::AddWaveMst(int _iWavwNum, wstring _wstrPrefName, Vec3 _vSpawnPos)
{
	//웨이브 vector에 몬스터 정보를 입력
	SpawnInfo Info;
	Info.PrefabName = _wstrPrefName;
	Info.SpawnPos = _vSpawnPos;
	m_vecWave[_iWavwNum].push_back(Info);
}

void CRoomScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iRoomNum, sizeof(int), 1, _File);
	fwrite(&m_iMaxWaveCount, sizeof(int), 1, _File);
	for (size_t i = 0; i < m_iMaxWaveCount; ++i)
	{
		size_t WaveSize = m_vecWave[i].size();
		fwrite(&WaveSize, sizeof(size_t), 1, _File);
		for (size_t j = 0; j < m_vecWave[i].size(); ++j)
		{
			SaveWString(m_vecWave[i][j].PrefabName, _File);
			fwrite(m_vecWave[i][j].SpawnPos, sizeof(Vec3), 1, _File);
		}
	}
}

void CRoomScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_iRoomNum, sizeof(int), 1, _File);
	fread(&m_iMaxWaveCount, sizeof(int), 1, _File);
	for (size_t i = 0; i < m_iMaxWaveCount; ++i)
	{
		size_t WaveSize = 0;
		fread(&WaveSize, sizeof(size_t), 1, _File);
		m_vecWave[i].resize(WaveSize);
		for (size_t j = 0; j < m_vecWave[i].size(); ++j)
		{
			LoadWString(m_vecWave[i][j].PrefabName, _File);
			fread(&m_vecWave[i][j].SpawnPos, sizeof(Vec3), 1, _File);
		}
	}
}
