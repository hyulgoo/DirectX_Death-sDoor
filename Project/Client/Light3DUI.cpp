#include "pch.h"
#include "Light3DUI.h"
#include <Engine/CLight3D.h>

Light3DUI::Light3DUI()
	: ComponentUI("##Light3D", COMPONENT_TYPE::LIGHT3D)
	, m_LightInfo{}
{
	SetName("Light3D");
}

Light3DUI::~Light3DUI()
{
}

int Light3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	m_LightInfo = GetTarget()->Light3D()->GetLightInfo();

	// Light Type
	int iLightType = m_LightInfo.LightType;
	ImGui::Text("LightType    ");
	ImGui::SameLine();

	const char* LightType[3] = { "Directional", "Point", "Spot" };
	if (ImGui::Combo("##LightType", &iLightType, LightType, 3, 3))
	{
		GetTarget()->Light3D()->SetLightType((LIGHT_TYPE)iLightType);
	}

	// Radius
	ImGui::Text("Radius       ");
	ImGui::SameLine();
	float fRadius = m_LightInfo.Radius;
	if (ImGui::SliderFloat("##LightRadius", &fRadius, 0.f, 2000.f))
	{
		GetTarget()->Light3D()->SetRadius(fRadius);
	}

	// Angle
	ImGui::Text("Angle        ");
	ImGui::SameLine();

	float fAngle = m_LightInfo.Angle * 180.f / XM_PI;

	if (ImGui::SliderFloat("##LightAngle", &fAngle, 0.f, 180.f))
	{
		GetTarget()->Light3D()->SetAngle(fAngle / 180.f * XM_PI);
	}
	//======
	// Color
	//======
	Vec4 vDiffuseColor = m_LightInfo.Color.vDiffuse * 255.f;
	float fDiffuse[4] = { vDiffuseColor.x, vDiffuseColor.y, vDiffuseColor.z, vDiffuseColor.w };

	Vec4 vSpecularColor = m_LightInfo.Color.vSpecular * 255.f;
	float fSpecular[4] = { vSpecularColor.x, vSpecularColor.y, vSpecularColor.z, vSpecularColor.w };

	Vec4 vAmbientColor = m_LightInfo.Color.vAmbient * 255;
	float fAmbient[4] = { vAmbientColor.x, vAmbientColor.y, vAmbientColor.z, vAmbientColor.w };

	// Diffuse Color
	ImGui::Text("DiffuseColor ");
	ImGui::SameLine();

	if (ImGui::SliderFloat4("##DiffuseColor", fDiffuse, 0.f, 255.f))
	{
		vDiffuseColor = Vec4(fDiffuse[0] / 255, fDiffuse[1] / 255, fDiffuse[2] / 255, fDiffuse[3] / 255);
		GetTarget()->Light3D()->SetLightDiffuse(vDiffuseColor);
	}

	// Specular Color
	ImGui::Text("SpecularColor");
	ImGui::SameLine();

	if (ImGui::SliderFloat4("##SpecularColor", fSpecular, 0.f, 255.f))
	{
		vSpecularColor = Vec4(fSpecular[0] / 255, fSpecular[1] / 255, fSpecular[2] / 255, fSpecular[3] / 255);
		GetTarget()->Light3D()->SetLightSpecular(vSpecularColor);
	}

	// Ambient Color
	ImGui::Text("AmbientColor ");
	ImGui::SameLine();

	if (ImGui::SliderFloat4("##AmbientColor", fAmbient, 0.f, 255.f))
	{
		vAmbientColor = Vec4(fAmbient[0] / 255, fAmbient[1] / 255, fAmbient[2] / 255, fAmbient[3] / 255);
		GetTarget()->Light3D()->SetLightAmbient(vAmbientColor);
	}
	
	return TRUE;
}
