#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>

Collider2DUI::Collider2DUI()
    : ComponentUI("##Coillider2D", COMPONENT_TYPE::COLLIDER2D)
{
    SetName("Coillider2D");
}

Collider2DUI::~Collider2DUI()
{
}

int Collider2DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;
	Vec3 vScale = GetTarget()->Collider2D()->GetOffsetScale();
	Vec3 vPos = GetTarget()->Collider2D()->GetOffsetPos();
	bool bAbs = GetTarget()->Collider2D()->IsAbsolute();
	int iabs = bAbs ? 1 : 0;
	COLLIDER2D_TYPE tShape = GetTarget()->Collider2D()->GetCollider2DType();

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Offset Pos", vPos);


	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Offset Scale", vScale);


	ImGui::Text("Absolute");
	ImGui::SameLine();
	static int elem = iabs;
	const char* elems_names[2] = { "FALSE", "TRUE" };
	const char* elem_name = (elem >= 0 && elem < 2) ? elems_names[elem] : "Unknown";
	ImGui::SliderInt("Absolute", &elem, 0, 1, elem_name);
	bAbs = elem ? true : false;

	ImGui::Text("Type    ");
	ImGui::SameLine();
	const char* item_names[] =
	{
		"RECT", "CIRCLE"
	};
	static int item_type = (int)tShape;
	static bool _disabled = false;
	ImGui::Combo("Collider2D Type", &item_type, item_names, IM_ARRAYSIZE(item_names), IM_ARRAYSIZE(item_names));


	ImGui::Text("Collision Count");
	ImGui::SameLine();
	int iCount = GetTarget()->Collider2D()->GetCollisionCount();
	ImGui::DragInt("##Collision Count", &iCount, ImGuiInputTextFlags_ReadOnly);

	GetTarget()->Collider2D()->SetOffsetScale(Vec2(vScale.x, vScale.y));
	GetTarget()->Collider2D()->SetOffsetPos(Vec2(vPos.x, vPos.y));
	GetTarget()->Collider2D()->SetAbsolute(bAbs);
	GetTarget()->Collider2D()->SetCollider2DType((COLLIDER2D_TYPE)item_type);

    return TRUE;
}