#include "pch.h"
#include "PrefabUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include "CLevelSaveLoad.h"
#include <Engine/CResMgr.h>

PrefabUI::PrefabUI()
    :ResUI(RES_TYPE::PREFAB)
{
    SetName("Prefab");
}

PrefabUI::~PrefabUI()
{

}

int PrefabUI::render_update()
{
    ResUI::render_update();

    // Prefab 이름
    ImGui::Text("Prefab          ");
    ImGui::SameLine();

    Ptr<CPrefab> pPrefab = (CPrefab*)GetTargetRes().Get();
    string strKey = string(pPrefab->GetKey().begin(), pPrefab->GetKey().end());
    ImGui::InputText("##PrefabUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    // Save 버튼
    ImGui::Text("Save            ");
    ImGui::SameLine();
    static char szEmtpy[30] = {};
    ImGui::InputText("##PrefabNewName", szEmtpy, 30);
    ImGui::SameLine();

    if (ImGui::Button("##PrefabSaveButton", ImVec2(18.f, 18.f)))
    {
        string strTemp = szEmtpy;
        wstring wstrTemp = wstring(strTemp.begin(), strTemp.end());

        CLevelSaveLoad::SavePrefab(L"prefab\\" + wstrTemp + L".prefab", (CPrefab*)GetTargetRes().Get());
    }

    // Spawn
    static int iSpawnLayer = 0;

    ImGui::Text("SpawnLayerIndex ");
    ImGui::SameLine();
    ImGui::SliderInt("##SpawnLayerIndex", &iSpawnLayer, 0, MAX_LAYER);

    static float SpawnPos[3] = { 0.f,0.f,0.f };
    ImGui::Text("SpawnPos        ");
    ImGui::SameLine();
    ImGui::DragFloat3("##SpawnPos", SpawnPos);
    ImGui::SameLine();

    if (ImGui::Button("##SpawnGameObjectButton", ImVec2(18.f, 18.f)))
    {
        /*CGameObject* proto = CLevelSaveLoad::LoadPrefab(GetTargetRes()->GetRelativePath());
        pPrefab->RegisterProtoObject(proto);
        SpawnGameObject(pPrefab->Instantiate(), Vec3(SpawnPos[0], SpawnPos[1], SpawnPos[2]), iSpawnLayer);*/
        CLevelSaveLoad::SpawnandReturnPrefab(GetTargetRes()->GetRelativePath(), iSpawnLayer, Vec3(SpawnPos[0], SpawnPos[1], SpawnPos[2]));
    }

    ImGui::TextColored(ImVec4(1, 1, 1, 1), "Layer Type Infos");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "0  : DEFAULT");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "1  : MAINCAMERA");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "2  : SUBCAMERA  ");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "3  : PLAYER");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "4  : MONSTER");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "5  : PLAYERPROJECTILE");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "6  : MONSTERPROJECTILE");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "7  : WALL");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "8  : ITEM");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "9  : GROUND");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "10 : FALLAREA");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "11 : LADDER");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "12 : ANCHOR");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "13 : BRAIZER");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "14 : NPC");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "31 : UI");
    return 0;
}