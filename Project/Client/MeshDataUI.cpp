#include "pch.h"
#include "MeshDataUI.h"
#include <Engine/CMeshData.h>
#include <Engine/CResMgr.h>

MeshDataUI::MeshDataUI()
    : ResUI(RES_TYPE::MESHDATA)
{
    SetName("MeshData");
}

MeshDataUI::~MeshDataUI()
{
}


int MeshDataUI::render_update()
{
    ResUI::render_update();
    ImGui::Text("Key          ");
    string strkey = string(GetTargetRes()->GetKey().begin(), GetTargetRes()->GetKey().end());
    ImGui::SameLine();
    ImGui::Text((char*)strkey.c_str());

    ImGui::Text("Relative Path");
    string strpath = string(GetTargetRes()->GetRelativePath().begin(), GetTargetRes()->GetRelativePath().end());
    ImGui::SameLine();
    ImGui::Text((char*)strpath.c_str());

    //사용하는 Mesh Key 정보
    Ptr<CMeshData> mdat = CResMgr::GetInst()->LoadFBX(GetTargetRes()->GetRelativePath());
    ImGui::TextColored(ImVec4(0.4, 0, 0.4, 1), "Mesh Key     ");
    string meshkey = string(mdat->GetMesh()->GetKey().begin(), mdat->GetMesh()->GetKey().end());
    ImGui::Text((char*)meshkey.c_str());

    //사용하는 Materials Key 정보
    ImGui::TextColored(ImVec4(0.4,0,0.4,1),"Mtrl Key     ");
    for (size_t i = 0; i < mdat->GetMaterials().size(); i++)
    {
        string matrialkey = string(mdat->GetMaterials()[i]->GetKey().begin(), mdat->GetMaterials()[i]->GetKey().end());
        ImGui::Text((char*)matrialkey.c_str());
    }
    
    return 0;
}