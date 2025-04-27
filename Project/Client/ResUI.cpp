#include "pch.h"
#include "ResUI.h"


ResUI::ResUI(RES_TYPE _type)
	: UI("##ResUI")	
	, m_ResType(_type)
{

}

ResUI::~ResUI()
{
}

int ResUI::render_update()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.639, 0.878, 0.39, 0.39));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.639, 0.878, 0.39, 0.39));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.639, 0.878, 0.39, 0.39));
	ImGui::Button(GetName().c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	return 0;
}