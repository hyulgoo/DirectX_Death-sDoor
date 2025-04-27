#include "pch.h"
#include "CEnterScript.h"
#include "CSpawnMgr.h"

CEnterScript::CEnterScript()
	: CScript((UINT)SCRIPT_TYPE::ENTERSCRIPT)
	, m_iRoomNum(-1)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
}

CEnterScript::~CEnterScript()
{
}

void CEnterScript::begin()
{
}

void CEnterScript::BeginOverlap(CCollider3D* _Other)
{
	CSpawnMgr::GetInst()->ActivateFence(m_iRoomNum, false);
	Destroy();
}

void CEnterScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iRoomNum, sizeof(int), 1, _File);
}

void CEnterScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_iRoomNum, sizeof(int), 1, _File);
}
