#include "pch.h"
#include "CBrazierScript.h"
#include "CSpawnMgr.h"

CBrazierScript::CBrazierScript()
	: CScript((UINT)SCRIPT_TYPE::BRAZIERSCRIPT)
{
}

CBrazierScript::~CBrazierScript()
{
}

void CBrazierScript::begin()
{
}

void CBrazierScript::tick()
{
}

void CBrazierScript::BeginOverlap(CCollider3D* _Other)
{
}

void CBrazierScript::EndOverlap(CCollider3D* _Other)
{
}

void CBrazierScript::SaveToLevelFile(FILE* _File)
{
}

void CBrazierScript::LoadFromLevelFile(FILE* _FILE)
{
}
