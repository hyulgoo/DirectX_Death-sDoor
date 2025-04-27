#include "pch.h"
#include "CHUDScript.h"
#include "CPlayerScript.h"
#include "CStateScript.h"
#include "CLevelSaveLoadInScript.h"

CHUDScript::CHUDScript()	:
	CScript(SCRIPT_TYPE::HUDSCRIPT)
{
}

CHUDScript::~CHUDScript()
{
}

void CHUDScript::begin()
{
	// 동적 재질 생성.
	int iMtrlCount = MeshRender()->GetMtrlCount();

	for (int i = 0; i < iMtrlCount; ++i)
	{
		MeshRender()->GetDynamicMaterial(i);
	}

}

void CHUDScript::tick()
{
	
}
