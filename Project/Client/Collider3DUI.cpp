#include "pch.h"
#include "Collider3DUI.h"
#include <Engine/CPhysXMgr.h>
#include <Engine/CCollider3D.h>

Collider3DUI::Collider3DUI()
	: ComponentUI("##Collider3D", COMPONENT_TYPE::COLLIDER3D),
	pPos(Vec3(0.f,0.f,0.f)),
	pRadius(0.f),
	createSphere(false),
	setRenderRigidbody(false)
{
	SetName("Coillider3D");
}

Collider3DUI::~Collider3DUI()
{
}

int Collider3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	Vec3 vScale = GetTarget()->Collider3D()->GetOffsetScale();
	Vec3 vPos = GetTarget()->Collider3D()->GetOffsetPos();
	bool bAbs = GetTarget()->Collider3D()->IsAbsolute();
	bool bDebugShape = GetTarget()->Collider3D()->IsDebugShape();
	COLLIDER3D_TYPE tShape = GetTarget()->Collider3D()->GetCollider3DType();

	// Position
	ImGui::Text("Position");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Offset Pos", vPos))
	{
		GetTarget()->Collider3D()->SetOffsetPos(Vec3(vPos.x, vPos.y, vPos.z));
	}

	// Scale
	ImGui::Text("Scale   ");
	ImGui::SameLine();
	if (ImGui::DragFloat3("##Offset Scale", vScale))
	{
		GetTarget()->Collider3D()->SetOffsetScale(Vec3(vScale.x, vScale.y, vScale.z));
	}

	// Absolute
	ImGui::Text("Absolute");
	ImGui::SameLine();
	if (ImGui::Checkbox("##Absolute", &bAbs))
	{
		GetTarget()->Collider3D()->SetAbsolute(bAbs);
	}

	// DebugShape
	ImGui::Text("DebugShape");
	ImGui::SameLine();
	if (ImGui::Checkbox("##DebugShape", &bDebugShape))
	{
		GetTarget()->Collider3D()->SetDebugShape(bDebugShape);
	}

	// Type
	ImGui::Text("Type    ");
	ImGui::SameLine();
	const char* ColliderType[2] = {"CUBE", "SPHERE"};
	static int CurType = (int)tShape;
	if (ImGui::Combo("##CollidereD Type", &CurType, ColliderType, 2, 2))
	{
		GetTarget()->Collider3D()->SetCollider3DType((COLLIDER3D_TYPE)CurType);
	}

	// Collision Count
	ImGui::Text("Collision Count");
	ImGui::SameLine();
	int iCount = GetTarget()->Collider3D()->GetCollisionCount();
	ImVec4 vColor = iCount == 0 ? ImVec4(0.f, 1.f, 0.f, 1.f) : ImVec4(1.f, 0.f, 0.f, 1.f);
	string Count = to_string(iCount);
	ImGui::TextColored(vColor, Count.c_str());
	

	

	return TRUE;
}
