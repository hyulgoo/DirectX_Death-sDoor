#include "pch.h"
#include "CCursorScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CDevice.h>

CCursorScript::CCursorScript()
	: CScript((UINT)CURSORSCRIPT)
{
	
}

CCursorScript::~CCursorScript()
{
}

void CCursorScript::begin()
{
	//ShowCursor(false);
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\Cursor.png", L"texture\\Cursor.png", 0));
}

void CCursorScript::tick()
{
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vRederSolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 vMainCamPos = CRenderMgr::GetInst()->GetMainCam()->GetOwner()->Transform()->GetWorldPos();

	Vec2 CurPos = Vec2(vRederSolution.x / 2.f, vRederSolution.y / 2.f);
	Vec2 vDefault = Vec2(0.f, -1.f);
	Vec2 TargetPos = Vec2(vMousePos.x, vMousePos.y);
	Vec2 vDir = Vec2(TargetPos.x - CurPos.x, TargetPos.y - CurPos.y);
	vDir.Normalize();
	float angle = (float)acos(vDir.Dot(vDefault));

	if (vDir.x > 0.f)
		angle = XM_2PI - angle;

	if (angle > XM_PI)
		angle = angle - XM_2PI;
	else if (angle < -XM_PI)
		angle = angle + XM_2PI;

	Transform()->SetRelativePos(vMousePos.x - vRederSolution.x / 2.f, -vMousePos.y + vRederSolution.y /2.f, 0.f);
	Transform()->SetRelativeRot(0.f, 0.f, angle);
}
