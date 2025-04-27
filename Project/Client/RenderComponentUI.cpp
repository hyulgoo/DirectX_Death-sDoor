#include "pch.h"
#include "RenderComponentUI.h"
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CRenderMgr.h>
#include <Engine\CResMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"

#include "TreeUI.h"
#include "ParamUI.h"

RenderComponentUI::RenderComponentUI()
	: UI("##RenderComponent")
	, m_Target(nullptr)
{
	SetName("RenderComponent");
}

RenderComponentUI::~RenderComponentUI()
{
}

int RenderComponentUI::render_update()
{

	if (nullptr == m_Target || nullptr == m_Component)
		return FALSE;

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.639, 0.878, 0.39, 0.39));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.639, 0.878, 0.39, 0.39));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.639, 0.878, 0.39, 0.39));
	ImGui::Button(GetName().c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	char szBuff[50] = {};

	Ptr<CMesh> pMesh = m_Component->GetMesh();
	Ptr<CMaterial> pMtrl = m_Component->GetMaterial(0);

	if (nullptr != pMesh)
	{
		ImGui::Text("Mesh    ");
		ImGui::SameLine();
		wstring wstrMeshKey = pMesh->GetKey();
		string strMeshKey = string(wstrMeshKey.begin(), wstrMeshKey.end());
		memcpy(szBuff, strMeshKey.data(), sizeof(char) * strMeshKey.length());
		ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

		// Mesh 드랍 체크
		if (ImGui::BeginDragDropTarget())
		{
			// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
			const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
			if (pPayLoad)
			{
				TreeNode* pNode = (TreeNode*)pPayLoad->Data;
				CRes* pRes = (CRes*)pNode->GetData();
				if (RES_TYPE::MESH == pRes->GetType())
				{
					m_Component->SetMesh((CMesh*)pRes);
				}
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::SameLine();
		// Mesh 선택
		if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
		{
			const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::MESH);

			ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
			pListUI->Reset("Mesh List", ImVec2(300.f, 500.f));
			for (const auto& pair : mapMesh)
			{
				pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
			}
			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&RenderComponentUI::SelectMesh);
		}
	}

	if (nullptr != pMtrl)
	{
		// Mtrl 이름
		ImGui::Text("Material");
		ImGui::SameLine();

		wstring wstrMtrlKey = pMtrl->GetKey();
		string strMtrlKey = string(wstrMtrlKey.begin(), wstrMtrlKey.end());
		memcpy(szBuff, strMtrlKey.data(), sizeof(char) * strMtrlKey.length());
		ImGui::InputText("##MtrlName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginDragDropTarget())
		{
			// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
			const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
			if (pPayLoad)
			{
				TreeNode* pNode = (TreeNode*)pPayLoad->Data;
				CRes* pRes = (CRes*)pNode->GetData();
				if (RES_TYPE::MATERIAL == pRes->GetType())
				{
					m_Component->SetMesh((CMesh*)pRes);
				}
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::SameLine();

		// Mtrl 선택 
		if (ImGui::Button("##MtrlSelectBtn", ImVec2(18, 18)))
		{
			const map<wstring, Ptr<CRes>>& mapMtrl = CResMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

			ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
			pListUI->Reset("Material", ImVec2(300.f, 500.f));
			for (const auto& pair : mapMtrl)
			{
				pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
			}

			// 항목 선택시 호출받을 델리게이트 등록
			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&RenderComponentUI::SelectMaterial);
		}

		// 출력할 TEX_PARAM 지정
		ImGui::Text("TEXPARAM");
		ImGui::SameLine();
		static int iTexparam = 0;
		string strTex[(int)TEX_PARAM::TEX_END] = { "TEX_0", "TEX_1", "TEX_2" , "TEX_3", "TEX_4", "TEX_5", "TEX_6", "TEX_7"
												   , "TEX_CUBE_0", "TEX_CUBE_0", "TEX_ARR_0", "TEX_ARR_1" };
		string strTexParamName = strTex[iTexparam];
		ImGui::SliderInt("##TexParm", &iTexparam, 0, 11, strTexParamName.c_str());

		// TEX_0 Texture 출력 및 선택
		Ptr<CTexture> pTEX = pMtrl->GetTexParam((TEX_PARAM)iTexparam);

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
			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&RenderComponentUI::SelectTexture);
		}
	}

	if(GetTarget()->GetRenderComponent()->IsUseFrustumCheck())
	{
		float fBounding = m_Component->GetBounding();
		ImGui::Text("Bounding");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##Bounding", &fBounding, 0.f, 1000.f))
			m_Component->SetBounding(fBounding);

		static bool bDrawBoundingBox = false;
		ImGui::SameLine();
		ImGui::Checkbox("##DrawBoundingBox", &bDrawBoundingBox);

		if (bDrawBoundingBox)
		{
			CCamera* pEditCam = CRenderMgr::GetInst()->GetMainCam();
			Vec3 Rot = pEditCam->Transform()->GetRelativeRot();
			DrawDebugCircle(m_Target->Transform()->GetWorldPos(), m_Component->GetBounding(), Vec4(0.f, 1.f, 0.f, 1.f), Rot);
		}
	}

	return TRUE;
}

void RenderComponentUI::SetTarget(CGameObject* _Target)
{
	m_Target = _Target;

	if (nullptr == m_Target)
	{
		SetActive(false);
		return;
	}
	bool Exist = false;
	for (UINT i = (UINT)COMPONENT_TYPE::MESHRENDER; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_Target->GetComponent((COMPONENT_TYPE)i))
		{
			m_Component = (CRenderComponent*)m_Target->GetComponent((COMPONENT_TYPE)i);
			Exist = true;
		}
	}

	if (Exist)
		SetActive(true);
	else
		SetActive(false);
}

void RenderComponentUI::SelectMesh(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(wstring(strKey.begin(), strKey.end()));
	m_Component->SetMesh(pMesh);
}

void RenderComponentUI::SelectMaterial(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstring(strKey.begin(), strKey.end()));
	m_Component->SetMaterial(pMtrl, 0);
}

void RenderComponentUI::SelectTexture(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	Ptr<CMaterial> pMtrl = m_Component->GetMaterial(0);
	pMtrl->SetTexParam(TEX_0, pTex);
}
