#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine\CMeshRender.h>
#include <Engine\CResMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"

#include "TreeUI.h"



MeshRenderUI::MeshRenderUI()
	: ComponentUI("##MeshRender", COMPONENT_TYPE::MESHRENDER)	
{
	SetName("MeshRender");
}

MeshRenderUI::~MeshRenderUI()
{
}


int MeshRenderUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	char szBuff[50] = {};

	Ptr<CMesh> pMesh = GetTarget()->MeshRender()->GetMesh();
	Ptr<CMaterial> pMtrl = GetTarget()->MeshRender()->GetMaterial(0);
		
	ImGui::Text("Mesh    ");
	ImGui::SameLine();	
	GetResKey(pMesh.Get(), szBuff, 50);
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
				GetTarget()->MeshRender()->SetMesh((CMesh*)pRes);
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();
	if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::MESH);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Mesh List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapMesh)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMesh);
	}
		
	ImGui::Text("Material");
	ImGui::SameLine();
	GetResKey(pMtrl.Get(), szBuff, 50);
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
				GetTarget()->MeshRender()->SetMesh((CMesh*)pRes);
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();

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
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMaterial);
	}
	/*int datai0 = 0;
	int datai1 = 0;
	int datai2 = 0;
	float dataf0 = 0.f;
	float dataf1 = 0.f;
	float dataf2 = 0.f;
	GetTarget()->MeshRender()->GetMaterial(0)->GetScalarParam(INT_0, &datai0);
	GetTarget()->MeshRender()->GetMaterial(0)->GetScalarParam(INT_1, &datai1);
	GetTarget()->MeshRender()->GetMaterial(0)->GetScalarParam(INT_2, &datai2);
	GetTarget()->MeshRender()->GetMaterial(0)->GetScalarParam(FLOAT_0, &dataf0);
	GetTarget()->MeshRender()->GetMaterial(0)->GetScalarParam(FLOAT_1, &dataf1);
	GetTarget()->MeshRender()->GetMaterial(0)->GetScalarParam(FLOAT_2, &dataf2);
	ImGui::InputInt("INT_0", &datai0);
	ImGui::InputInt("INT_1", &datai1);
	ImGui::InputInt("INT_2", &datai2);
	ImGui::InputFloat("FLOAT_0", &dataf0);
	ImGui::InputFloat("FLOAT_1", &dataf1);
	ImGui::InputFloat("FLOAT_2", &dataf2);
	GetTarget()->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &datai0);
	GetTarget()->MeshRender()->GetMaterial(0)->SetScalarParam(INT_1, &datai1);
	GetTarget()->MeshRender()->GetMaterial(0)->SetScalarParam(INT_2, &datai2);
	GetTarget()->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &dataf0);
	GetTarget()->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_1, &dataf1);
	GetTarget()->MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_2, &dataf2);*/
	
	return TRUE;
}

void MeshRenderUI::SelectMesh(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::SelectMaterial(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->MeshRender()->SetMaterial(pMtrl,0);
}