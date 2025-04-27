#include "pch.h"
#include "CAnchorScript.h"

CAnchorScript::CAnchorScript()
	: CScript((UINT)SCRIPT_TYPE::ANCHORSCRIPT)
{
}

CAnchorScript::~CAnchorScript()
{
}

void CAnchorScript::begin()
{
}

void CAnchorScript::tick()
{
}

void CAnchorScript::BeginOverlap(CCollider3D* _Other)
{
}

void CAnchorScript::EndOverlap(CCollider3D* _Other)
{
}

void CAnchorScript::SaveToLevelFile(FILE* _File)
{
}

void CAnchorScript::LoadFromLevelFile(FILE* _FILE)
{
}
