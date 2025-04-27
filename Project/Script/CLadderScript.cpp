#include "pch.h"
#include "CLadderScript.h"
#include "CStateScript.h"
#include "CLevelSaveLoadInScript.h"

CLadderScript::CLadderScript()
	: CScript((UINT)SCRIPT_TYPE::LADDERSCRIPT)
	, m_pInterectionImage(nullptr)
	, m_fHeight(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fHeight, "Height");
}

CLadderScript::~CLadderScript()
{
}

void CLadderScript::BeginOverlap(CCollider3D* _Other)
{
}

void CLadderScript::OnOverlap(CCollider3D* _Other)
{
}

void CLadderScript::EndOverlap(CCollider3D* _Other)
{
}

void CLadderScript::SaveToLevelFile(FILE* _FILE)
{
	fwrite(&m_fHeight, sizeof(float), 1, _FILE);
}

void CLadderScript::LoadFromLevelFile(FILE* _FILE)
{
	fread(&m_fHeight, sizeof(float), 1, _FILE);
}
