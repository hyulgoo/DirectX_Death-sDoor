#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CResMgr.h>
#include "TreeUI.h"
#include "ListUI.h"
#include "commdlg.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("##ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
	, m_pParticleSystem(nullptr)
	, m_tModuleData{}
	, m_iEmissive(0)
{
	SetName("ParticleSysyem");
}

ParticleSystemUI::~ParticleSystemUI()
{
}

int ParticleSystemUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	if (nullptr == m_pParticleSystem)
	{
		m_pParticleSystem = GetTarget()->ParticleSystem();
	}
	m_tModuleData = m_pParticleSystem->GetModuleData();

	SaveParticle();

	LoadParticle();

	// 각 모듈 OnOff 기능, On 때만 해당 모듈 설정이 켜지도록 함.
	ModuleOnOff("Spawn Module", "##Spawn_Module", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN])
		SpawnModule();

	ImGui::NewLine();
	ImGui::Separator();

	ModuleOnOff("Scale Change", "##Scale_Change", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE])
		ScaleModule();

	ImGui::NewLine();
	ImGui::Separator();

	ModuleOnOff("Color Change", "##Color_Change", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE])
		ColorModule();

	ImGui::NewLine();
	ImGui::Separator();

	// Emissive 여부
	bool bEmissive = GetTarget()->ParticleSystem()->GetEmissive();
	ImGui::Checkbox("Emissive", &bEmissive);
	if (bEmissive)
		m_iEmissive = 1;
	else
		m_iEmissive = 0;
	GetTarget()->ParticleSystem()->SetEmissive(m_iEmissive);

	ImGui::NewLine();
	ImGui::Separator();

	ModuleOnOff("Velocity", "##Velocity_Module", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY])
		AddVelocityModule();

	ModuleOnOff("Drag        ", "##Drag_Module", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG])
		DragModule();

	ImGui::NewLine();
	ImGui::Separator();

	ModuleOnOff("Random Force", "##Random_Force", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE])
		RandomForceModule();

	ImGui::NewLine();
	ImGui::Separator();

	ModuleOnOff("Render", "##Render_Module", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER])
		RenderModule();

	ImGui::NewLine();
	ImGui::Separator();

	ModuleOnOff("Animation    ", "##Particle_Animation", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION])
		AnimationModule();


	m_pParticleSystem->SetModuleData(m_tModuleData);

	return TRUE;
}

void ParticleSystemUI::ModuleOnOff(string _strName, string _strID, int& _ModuleCheck)
{
	ImVec4 Color = ImVec4(255.f / 255.f, 105.f / 255.f, 97.f / 255, 1.f);
	if (1 == _ModuleCheck)
		Color = ImVec4(152.f / 255.f, 251.f / 255.f, 152.f / 255, 1.f);
	ImGui::TextColored(ImVec4(255.f / 255.f, 174.f / 255.f, 105.f / 255, 1.f), _strName.c_str());
	ImGui::SameLine();
	char szbuffer[50] = { "ON/OFF##" };
	strcpy_s(szbuffer, _strName.c_str());
	strcpy_s(szbuffer, _strID.c_str());
	if (ImGui::ColorButton(szbuffer, Color, ImGuiColorEditFlags_::ImGuiColorEditFlags_NoTooltip, ImVec2(15.f, 15.f)))
	{
		_ModuleCheck = _ModuleCheck ? false : true;
	}
}

void ParticleSystemUI::SetTexture()
{
	Ptr<CTexture> pTEX = GetTarget()->ParticleSystem()->GetTex();

	ImGui::Spacing();
	ImGui::Text("Texture ");
	ImGui::SameLine();
	if (nullptr == pTEX)
	{
		ImGui::Image((ImTextureID)0, ImVec2(75.f, 75.f));
	}
	else
	{
		ImGui::Image((ImTextureID)pTEX->GetSRV().Get(), ImVec2(75.f, 75.f));
	}


	ImGui::SameLine();

	if (ImGui::Button("##TextureSelectBtn", ImVec2(18.f, 18.f)))
	{
		const map<wstring, Ptr<CRes>>& AllTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->SetModal(false);
		pListUI->Reset("Texture List", ImVec2(300.f, 500.f));
		for (const auto& pair : AllTex)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&ParticleSystemUI::SelectTexture);
	}
	//========
	Ptr<CTexture> pTex = GetTarget()->ParticleSystem()->GetTex();
	char szbuff[50] = {};
	GetResKey(pTex.Get(), szbuff, 50);
	ImGui::Text("Texture     ");
	ImGui::SameLine();
	ImGui::InputText("##TexName", szbuff, ImGuiInputTextFlags_ReadOnly);
}

void ParticleSystemUI::SelectTexture(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->ParticleSystem()->SetTex(pTex);
}

void ParticleSystemUI::SpawnModule()
{
	ImGui::Text("MaxCount    ");
	ImGui::SameLine();
	ImGui::DragInt("##Spawn_MaxCount", &m_tModuleData.iMaxParticleCount);

	ImGui::Text("SpawnRate   ");
	ImGui::SameLine();
	ImGui::DragInt("##Spawn_SpawnRate", &m_tModuleData.SpawnRate);

	//ImVec4 SpawnColor = ImVec4(m_tModuleData.vSpawnColor.x, m_tModuleData.vSpawnColor.y, m_tModuleData.vSpawnColor.z, m_tModuleData.vSpawnColor.z);
	ImGui::Text("Color       ");
	ImGui::SameLine();
	ImGui::DragFloat4("##Spawn_Color", &m_tModuleData.vSpawnColor.x);
	//m_tModuleData.vSpawnColor = Vec4(SpawnColor.x, SpawnColor.y, SpawnColor.z, SpawnColor.w);

	ImGui::Text("MinScale    ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Spawn_MinScale", &m_tModuleData.vSpawnScaleMin.x);

	ImGui::Text("MaxScale    ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Spawn_MaxScale", &m_tModuleData.vSpawnScaleMax.x);

	ImGui::Text("ShapeType   ");
	ImGui::SameLine();
	string SpawnType[2] = { "BOX", "SPHERE" };
	string TypeName = SpawnType[m_tModuleData.SpawnShapeType];
	ImGui::SliderInt("##Spawn_ShapeType", &m_tModuleData.SpawnShapeType, 0, 1, TypeName.c_str());

	ImGui::Text("Space       ");
	ImGui::SameLine();
	string SpaceType[2] = { "World", "Local" };
	string SpaceName = SpaceType[m_tModuleData.Space];
	ImGui::SliderInt("##Spawn_Space", &m_tModuleData.Space, 0, 1, SpaceName.c_str());

	if (0 == m_tModuleData.SpawnShapeType)
	{
		ImGui::Text("SpawnRange  ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Spawn_BoxRange", &m_tModuleData.vBoxShapeScale.x);
	}

	else if (1 == m_tModuleData.SpawnShapeType)
	{
		ImGui::Text("SpawnRange  ");
		ImGui::SameLine();
		ImGui::DragFloat("##Spawn_SphereRadius", &m_tModuleData.fSphereShapeRadius);
	}

	ImGui::Text("MinTime     ");
	ImGui::SameLine();
	ImGui::DragFloat("##Spawn_MinTime", &m_tModuleData.MinLifeTime);

	ImGui::Text("MaxTime     ");
	ImGui::SameLine();
	ImGui::DragFloat("##Spawn_MaxTime", &m_tModuleData.MaxLifeTime);

	//Texture 사용
	SetTexture();
}

void ParticleSystemUI::ScaleModule()
{
	ImGui::Text("StartScale ");
	ImGui::SameLine();
	ImGui::DragFloat("##Start_Scale", &m_tModuleData.StartScale);

	ImGui::Text("EndScale   ");
	ImGui::SameLine();
	ImGui::DragFloat("##End_Scale", &m_tModuleData.EndScale);
}

void ParticleSystemUI::ColorModule()
{
	ImGui::Text("StartColor  ");
	ImGui::SameLine();
	// Generate a default palette. The palette will persist and can be edited.
	static bool saved_palette_init = true;
	static ImVec4 saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f; // Alpha
		}
		saved_palette_init = false;
	}

	static ImVec4 color = ImVec4(m_tModuleData.vStartColor.x, m_tModuleData.vStartColor.y, m_tModuleData.vStartColor.z, m_tModuleData.vStartColor.w);
	ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_HDR;
	static ImVec4 backup_color;
	bool open_popup = ImGui::ColorButton("MyColor##1", color, misc_flags);
	if (open_popup)
	{
		ImGui::OpenPopup("mypicker##1");
		backup_color = ImVec4(m_tModuleData.vStartColor.x, m_tModuleData.vStartColor.y, m_tModuleData.vStartColor.z, m_tModuleData.vStartColor.w);
	}
	if (ImGui::BeginPopup("mypicker##1"))
	{
		ImGui::Text("START COLOR PALETTE");
		ImGui::Separator();
		ImGui::ColorPicker4("##picker1", (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
		ImGui::SameLine();

		ImGui::BeginGroup(); // Lock X position
		ImGui::Text("Current##1");
		ImGui::ColorButton("##current1", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
		ImGui::Text("Previous##1");
		if (ImGui::ColorButton("##previous1", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
			color = backup_color;
		ImGui::Separator();
		ImGui::Text("Palette##1");
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::PushID(n);
			if ((n % 8) != 0)
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

			ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
			if (ImGui::ColorButton("##palette1", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
				color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

			// Allow user to drop colors into each palette entry. Note that ColorButton() is already a
			// drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
		m_tModuleData.vStartColor = Vec4(color.x, color.y, color.z, color.w);
	}

	ImGui::Text("EndColor    ");
	ImGui::SameLine();

	// Generate a default palette. The palette will persist and can be edited.
	static bool saved_palette_init1 = true;
	static ImVec4 saved_palette1[32] = {};
	if (saved_palette_init1)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette1); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette1[n].x, saved_palette1[n].y, saved_palette1[n].z);
			saved_palette1[n].w = 1.0f; // Alpha
		}
		saved_palette_init1 = false;
	}

	static ImVec4 color1 = ImVec4(m_tModuleData.vEndColor.x, m_tModuleData.vEndColor.y, m_tModuleData.vEndColor.z, m_tModuleData.vEndColor.w);
	ImGuiColorEditFlags misc_flags1 = ImGuiColorEditFlags_HDR;
	static ImVec4 backup_color1;
	bool open_popup1 = ImGui::ColorButton("MyColor##2", color1, misc_flags1);
	if (open_popup1)
	{
		ImGui::OpenPopup("mypicker##2");
		backup_color1 = ImVec4(m_tModuleData.vEndColor.x, m_tModuleData.vEndColor.y, m_tModuleData.vEndColor.z, m_tModuleData.vEndColor.w);
	}
	if (ImGui::BeginPopup("mypicker##2"))
	{
		ImGui::Text("END COLOR PALETTE");
		ImGui::Separator();
		ImGui::ColorPicker4("##picker2", (float*)&color1, misc_flags1 | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
		ImGui::SameLine();

		ImGui::BeginGroup(); // Lock X position
		ImGui::Text("Current##2");
		ImGui::ColorButton("##current2", color1, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
		ImGui::Text("Previous##2");
		if (ImGui::ColorButton("##previous2", backup_color1, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
			color1 = backup_color1;
		ImGui::Separator();
		ImGui::Text("Palette##2");
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette1); n++)
		{
			ImGui::PushID(n);
			if ((n % 8) != 0)
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

			ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
			if (ImGui::ColorButton("##palette2", saved_palette1[n], palette_button_flags, ImVec2(20, 20)))
				color1 = ImVec4(saved_palette1[n].x, saved_palette1[n].y, saved_palette1[n].z, color1.w); // Preserve alpha!

			// Allow user to drop colors into each palette entry. Note that ColorButton() is already a
			// drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
					memcpy((float*)&saved_palette1[n], payload->Data, sizeof(float) * 3);
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
					memcpy((float*)&saved_palette1[n], payload->Data, sizeof(float) * 4);
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
		m_tModuleData.vEndColor = Vec4(color1.x, color1.y, color1.z, color1.w);
	}
}

void ParticleSystemUI::AddVelocityModule()
{
	ImGui::Text("VelocityType");
	ImGui::SameLine();
	int elem = m_tModuleData.AddVelocityType;
	const char* elems_names[4] = { "From Center", "To Center", "Fixed Direction", "test" };
	const char* elem_name = (elem >= 0 && elem < 4) ? elems_names[elem] : "Unknown";
	ImGui::SliderInt("##Velocity_Type", &m_tModuleData.AddVelocityType, 0, 3, elem_name);

	ImGui::Text("Speed       ");
	ImGui::SameLine();
	ImGui::DragFloat("##VelocitySpeed", &m_tModuleData.Speed);

	ImGui::Text("VelocityDir ");
	ImGui::SameLine();
	ImGui::DragFloat4("##VelocityDir", &m_tModuleData.vVelocityDir.x);

	ImGui::Text("OffsetAngle ");
	ImGui::SameLine();
	ImGui::DragFloat("##Offset_Angle", &m_tModuleData.OffsetAngle);
}

void ParticleSystemUI::DragModule()
{
	ImGui::Text("StartDrag   ");
	ImGui::SameLine();
	ImGui::DragFloat("##Start_Drag", &m_tModuleData.StartDrag);

	ImGui::Text("EndDrag     ");
	ImGui::SameLine();
	ImGui::DragFloat("##End_Drag", &m_tModuleData.EndDrag);
}

void ParticleSystemUI::RandomForceModule()
{
	ImGui::Text("Noise Term  ");
	ImGui::SameLine();
	ImGui::DragFloat("##Noise_Term", &m_tModuleData.fNoiseTerm);

	ImGui::Text("Noise Force ");
	ImGui::SameLine();
	ImGui::DragFloat("##Noise_Force", &m_tModuleData.fNoiseForce);
}

void ParticleSystemUI::RenderModule()
{
	ImGui::Text("Velocity Alignment    ");
	ImGui::SameLine();
	ImGui::DragInt("##Velocity_Alignment", &m_tModuleData.VelocityAlignment);

	ImGui::Text("Velocity Scale    ");
	ImGui::SameLine();
	ImGui::DragInt("##Velocity_Scale", &m_tModuleData.VelocityScale);

	ImGui::Text("MaxScale    ");
	ImGui::SameLine();
	ImGui::DragFloat3("##MaxVelocityScale", &m_tModuleData.vMaxVelocityScale.x);

	ImGui::Text("MaxSpeed    ");
	ImGui::SameLine();
	ImGui::DragFloat("##MaxSpeed", &m_tModuleData.vMaxSpeed);
}

void ParticleSystemUI::VelocityScaleModule()
{

}

void ParticleSystemUI::AnimationModule()
{
	Ptr<CTexture> Atlas = m_pParticleSystem->GetTex();
	Vec2 TexSize = Vec2(Atlas->Width(), Atlas->Height());

	ImGui::Text("AnimInfo  ");

	Vec2 CurLeftTop = m_tModuleData.vLeftTop * TexSize;
	Vec2 CurSlice = m_tModuleData.vSlice * TexSize;
	Vec2 CurOffset = m_tModuleData.vOffset * TexSize;

	ImGui::Text("FrmNum    ");
	ImGui::SameLine();
	ImGui::DragInt("##AnimFrmNum", &m_tModuleData.iFrmCount, 1.f);

	ImGui::Text("XCount    ");
	ImGui::SameLine();
	ImGui::DragInt("##AnimXCount", &m_tModuleData.iXCount, 1.f);

	ImGui::Text("LeftTop   ");
	ImGui::SameLine();
	ImGui::DragFloat2("##AnimLeftTop", &CurLeftTop.x);

	ImGui::Text("Slice     ");
	ImGui::SameLine();
	ImGui::DragFloat2("##AnimSlice", &CurSlice.x);

	ImGui::Text("Offset    ");
	ImGui::SameLine();
	ImGui::DragFloat2("##AnimOffset", &CurOffset.x);

	m_tModuleData.vLeftTop = CurLeftTop / TexSize;
	m_tModuleData.vSlice = CurSlice / TexSize;
	m_tModuleData.vOffset = CurOffset / TexSize;
}

void ParticleSystemUI::SaveParticle()
{
	//Save Particle
	if (ImGui::Button("Save##Particle", ImVec2(100, 18)))
	{
		OPENFILENAME ofn = {};

		wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
		strFolderpath += L"texture\\particle\\";

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"Particle\0*.particle\0ALL\0*.*";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = strFolderpath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetSaveFileName(&ofn))
			return;

		// 파일 입출력
		FILE* pFile = nullptr;
		errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"wb");

		if (nullptr == pFile)
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Particle Save 실패, Error Number : %d", iErrNum);
			MessageBox(nullptr, szStr, L"파티클 저장 실패", MB_OK);
			return;
		}

		wchar_t szNum[50] = {};
		size_t iFrmCount = m_tModuleData.AddVelocityType;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwrite(&m_iEmissive, sizeof(int), 1, pFile); 
		fwprintf_s(pFile, L"[AddVelocityType]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[StartDrag]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.StartDrag);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[EndDrag]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.EndDrag);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[StartScale]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.StartScale);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[EndScale]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.EndScale);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[fNoiseForce]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.fNoiseForce);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[fNoiseTerm]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.fNoiseTerm);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[fSphereShapeRadius]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.fSphereShapeRadius);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.iMaxParticleCount;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[iMaxParticleCount]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[MaxLifeTime]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.MaxLifeTime);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[MinLifeTime]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.MinLifeTime);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=ADDVELOCITY]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=COLOR_CHANGE]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=DRAG]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=NOISE_FORCE]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=PARTICLE_SPAWN]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=RENDER]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=ANIMATION]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE];
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[ModuleCheck=SCALE_CHANGE]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[OffsetAngle]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.OffsetAngle);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.Space;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[Space]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.SpawnRate;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[SpawnRate]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.SpawnShapeType;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[SpawnShapeType]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[Speed]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.Speed);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[vBoxShapeScale]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f\n", m_tModuleData.vBoxShapeScale.x, m_tModuleData.vBoxShapeScale.y, m_tModuleData.vBoxShapeScale.z);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.VelocityAlignment;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[VelocityAlignment]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.VelocityScale;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[VelocityScale]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[vStartColor]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_tModuleData.vStartColor.x, m_tModuleData.vStartColor.y, m_tModuleData.vStartColor.z, m_tModuleData.vStartColor.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vEndColor]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_tModuleData.vEndColor.x, m_tModuleData.vEndColor.y, m_tModuleData.vEndColor.z, m_tModuleData.vEndColor.w);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[vMaxSpeed]\n");
		fwprintf_s(pFile, L"%.2f\n", m_tModuleData.vMaxSpeed);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vMaxVelocityScale]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_tModuleData.vMaxVelocityScale.x, m_tModuleData.vMaxVelocityScale.y, m_tModuleData.vMaxVelocityScale.z, m_tModuleData.vMaxVelocityScale.w);
		fwprintf_s(pFile, L"\n\n");

		//Animation module members
		iFrmCount = m_tModuleData.iFrmCount;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[FrmCount]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		iFrmCount = m_tModuleData.iXCount;
		_itow_s((int)iFrmCount, szNum, 50, 10);//정수를 문자열로 변환
		fwprintf_s(pFile, L"[XCount]\n");
		fwprintf_s(pFile, szNum);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[LeftTop]\n");
		fwprintf_s(pFile, L"%.2f %.2f \n", m_tModuleData.vLeftTop.x, m_tModuleData.vLeftTop.y);
		fwprintf_s(pFile, L"\n\n");
		
		fwprintf_s(pFile, L"[Slice]\n");
		fwprintf_s(pFile, L"%.2f %.2f \n", m_tModuleData.vSlice.x, m_tModuleData.vSlice.y);
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[Offset]\n");
		fwprintf_s(pFile, L"%.2f %.2f \n", m_tModuleData.vOffset.x, m_tModuleData.vOffset.y);
		fwprintf_s(pFile, L"\n\n");


		fwprintf_s(pFile, L"[vSpawnColor]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_tModuleData.vSpawnColor.x, m_tModuleData.vSpawnColor.y, m_tModuleData.vSpawnColor.z, m_tModuleData.vSpawnColor.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vSpawnScaleMax]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_tModuleData.vSpawnScaleMax.x, m_tModuleData.vSpawnScaleMax.y, m_tModuleData.vSpawnScaleMax.z, m_tModuleData.vSpawnScaleMax.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vSpawnScaleMin]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_tModuleData.vSpawnScaleMin.x, m_tModuleData.vSpawnScaleMin.y, m_tModuleData.vSpawnScaleMin.z, m_tModuleData.vSpawnScaleMin.w);
		fwprintf_s(pFile, L"\n\n");
		fwprintf_s(pFile, L"[vVelocityDir]\n");
		fwprintf_s(pFile, L"%.2f %.2f %.2f %.2f\n", m_tModuleData.vVelocityDir.x, m_tModuleData.vVelocityDir.y, m_tModuleData.vVelocityDir.z, m_tModuleData.vVelocityDir.w);
		fwprintf_s(pFile, L"\n\n");

		//사용할 texture 저장
		fwprintf_s(pFile, L"[Texture_KEY]\n");
		fwprintf_s(pFile, pTex->GetKey().c_str());
		fwprintf_s(pFile, L"\n\n");

		fwprintf_s(pFile, L"[Texture_PATH]\n");
		fwprintf_s(pFile, pTex->GetRelativePath().c_str());
		fwprintf_s(pFile, L"\n\n");

		fclose(pFile);
	}
}

void ParticleSystemUI::LoadParticle()
{
	//Load
	ImGui::SameLine();
	if (ImGui::Button("Load##Particle", ImVec2(100, 18)))
	{
		OPENFILENAME ofn = {};

		wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
		strFolderpath += L"texture\\particle\\";

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"Particle\0*.particle\0ALL\0*.*";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = strFolderpath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetOpenFileName(&ofn))
			return;
		// 파일 입출력
		FILE* pFile = nullptr;
		errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"rb");

		if (nullptr == pFile)
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Particle Load 실패, Error Number : %d", iErrNum);
			MessageBox(nullptr, szStr, L"파티클 불러오기 실패", MB_OK);
			return;
		}
		wstring strTextureKey;
		wstring strTextureRelativePath;
		while (true)
		{
			fread(&m_iEmissive, sizeof(int), 1, pFile);
			GetTarget()->ParticleSystem()->SetEmissive(m_iEmissive);
			wchar_t szBuffer[256] = {};
			fwscanf_s(pFile, L"%s", szBuffer, 256);//문자열 변수를 만들어 문자열을 읽게 할 건데

			if (!wcscmp(szBuffer, L"[AddVelocityType]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.AddVelocityType);
			}

			else if (!wcscmp(szBuffer, L"[StartDrag]"))
			{
				fwscanf_s(pFile, L"%f.2", &m_tModuleData.StartDrag);
			}

			else if (!wcscmp(szBuffer, L"[EndDrag]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.EndDrag);
			}

			else if (!wcscmp(szBuffer, L"[StartScale]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.StartScale);
			}

			else if (!wcscmp(szBuffer, L"[EndScale]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.EndScale);
			}

			else if (!wcscmp(szBuffer, L"[fNoiseForce]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.fNoiseForce);
			}

			else if (!wcscmp(szBuffer, L"[fNoiseTerm]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.fNoiseTerm);
			}

			else if (!wcscmp(szBuffer, L"[fSphereShapeRadius]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.fSphereShapeRadius);
			}

			else if (!wcscmp(szBuffer, L"[iMaxParticleCount]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.iMaxParticleCount);
			}

			else if (!wcscmp(szBuffer, L"[MaxLifeTime]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.MaxLifeTime);
			}

			else if (!wcscmp(szBuffer, L"[MinLifeTime]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.MinLifeTime);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=PARTICLE_SPAWN]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=COLOR_CHANGE]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=SCALE_CHANGE]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=ADDVELOCITY]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=DRAG]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=NOISE_FORCE]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=RENDER]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER]);
			}

			else if (!wcscmp(szBuffer, L"[ModuleCheck=ANIMATION]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION]);
			}

			else if (!wcscmp(szBuffer, L"[OffsetAngle]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.OffsetAngle);
			}

			else if (!wcscmp(szBuffer, L"[Space]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.Space);
			}

			else if (!wcscmp(szBuffer, L"[Space]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.Space);
			}

			else if (!wcscmp(szBuffer, L"[SpawnRate]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.SpawnRate);
			}

			else if (!wcscmp(szBuffer, L"[SpawnShapeType]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.SpawnShapeType);
			}

			else if (!wcscmp(szBuffer, L"[Speed]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.Speed);
			}

			else if (!wcscmp(szBuffer, L"[vBoxShapeScale]"))
			{
				fwscanf_s(pFile, L"%f%f%f", &m_tModuleData.vBoxShapeScale.x, &m_tModuleData.vBoxShapeScale.y, &m_tModuleData.vBoxShapeScale.z);
			}

			else if (!wcscmp(szBuffer, L"[VelocityAlignment]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.VelocityAlignment);
			}

			else if (!wcscmp(szBuffer, L"[VelocityScale]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.VelocityScale);
			}

			else if (!wcscmp(szBuffer, L"[vStartColor]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_tModuleData.vStartColor.x, &m_tModuleData.vStartColor.y, &m_tModuleData.vStartColor.z, &m_tModuleData.vStartColor.w);
			}

			else if (!wcscmp(szBuffer, L"[vEndColor]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_tModuleData.vEndColor.x, &m_tModuleData.vEndColor.y, &m_tModuleData.vEndColor.z, &m_tModuleData.vEndColor.w);
			}

			else if (!wcscmp(szBuffer, L"[vMaxSpeed]"))
			{
				fwscanf_s(pFile, L"%f", &m_tModuleData.vMaxSpeed);
			}

			else if (!wcscmp(szBuffer, L"[FrmCount]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.iFrmCount);
			}

			else if (!wcscmp(szBuffer, L"[XCount]"))
			{
				fwscanf_s(pFile, L"%d", &m_tModuleData.iXCount);
			}
			else if (!wcscmp(szBuffer, L"[LeftTop]"))
			{
				fwscanf_s(pFile, L"%f%f", &m_tModuleData.vLeftTop.x, &m_tModuleData.vLeftTop.y);
			}
			else if (!wcscmp(szBuffer, L"[Slice]"))
			{
				fwscanf_s(pFile, L"%f%f", &m_tModuleData.vSlice.x, &m_tModuleData.vSlice.y);
			}
			else if (!wcscmp(szBuffer, L"[Offset]"))
			{
				fwscanf_s(pFile, L"%f%f", &m_tModuleData.vOffset.x, &m_tModuleData.vOffset.y);
			}

			else if (!wcscmp(szBuffer, L"[vMaxVelocityScale]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_tModuleData.vMaxVelocityScale.x, &m_tModuleData.vMaxVelocityScale.y, &m_tModuleData.vMaxVelocityScale.z, &m_tModuleData.vMaxVelocityScale.w);
			}

			else if (!wcscmp(szBuffer, L"[vSpawnColor]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_tModuleData.vSpawnColor.x, &m_tModuleData.vSpawnColor.y, &m_tModuleData.vSpawnColor.z, &m_tModuleData.vSpawnColor.w);
			}

			else if (!wcscmp(szBuffer, L"[vSpawnScaleMax]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_tModuleData.vSpawnScaleMax.x, &m_tModuleData.vSpawnScaleMax.y, &m_tModuleData.vSpawnScaleMax.z, &m_tModuleData.vSpawnScaleMax.w);
			}

			else if (!wcscmp(szBuffer, L"[vSpawnScaleMin]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_tModuleData.vSpawnScaleMin.x, &m_tModuleData.vSpawnScaleMin.y, &m_tModuleData.vSpawnScaleMin.z, &m_tModuleData.vSpawnScaleMin.w);
			}

			else if (!wcscmp(szBuffer, L"[vVelocityDir]"))
			{
				fwscanf_s(pFile, L"%f%f%f%f", &m_tModuleData.vVelocityDir.x, &m_tModuleData.vVelocityDir.y, &m_tModuleData.vVelocityDir.z, &m_tModuleData.vVelocityDir.w);
			}
			else if (!wcscmp(szBuffer, L"[Texture_KEY]"))
			{
				fwscanf_s(pFile, L"%s", szBuffer, 256);
				strTextureKey = szBuffer;
			}

			else if (!wcscmp(szBuffer, L"[Texture_PATH]"))
			{
				fwscanf_s(pFile, L"%s", szBuffer, 256);
				strTextureRelativePath = szBuffer;
				break;
			}
		}

		GetTarget()->ParticleSystem()->SetModuleData(m_tModuleData);
		pTex = CResMgr::GetInst()->FindRes<CTexture>(strTextureKey).Get();
		GetTarget()->ParticleSystem()->SetTex(pTex);

		fclose(pFile);
	}
}
