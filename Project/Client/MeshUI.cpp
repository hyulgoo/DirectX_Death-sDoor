#include "pch.h"
#include "MeshUI.h"

#include <Engine\CMesh.h>
#include <Engine/CResMgr.h>
#include <Engine/components.h>

MeshUI::MeshUI()
    : ResUI(RES_TYPE::MESH)
{
    SetName("Mesh");
}

MeshUI::~MeshUI()
{
}

int MeshUI::render_update()
{
    ResUI::render_update();

    ImGui::Text("Mesh     ");
    ImGui::SameLine();

    Ptr<CMesh> pMesh = (CMesh*)GetTargetRes().Get();
    string strKey = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
    ImGui::InputText("##MeshUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    
    if (0 != pMesh->GetBoneCount())
    {
        int bonesize = (int)pMesh->GetBoneCount();
        ImGui::Text("Bone Size");
        ImGui::SameLine();
        ImGui::InputInt("##BoneSize", &bonesize,ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
    }

    if (pMesh->IsAnimMesh())
    {
       ImGui::Text("Anim Name");
       ImGui::SameLine();
       const vector<tMTAnimClip>* clip = pMesh->GetAnimClip();
       for (size_t i = 0; i < clip->size(); i++)
       {
           string animName = string((*clip)[i].strAnimName.begin(), (*clip)[i].strAnimName.end());
           ImGui::InputText("##AnimName", (char*)animName.c_str(), animName.length());

           ImGui::Text("Srt Frame");
           ImGui::SameLine();
           int startFrame = (*clip)[i].iStartFrame;
           ImGui::InputInt("##startFrame", &startFrame, ImGuiInputTextFlags_None);

           ImGui::Text("End Frame");
           ImGui::SameLine();
           int endFrame = (*clip)[i].iEndFrame;
           ImGui::InputInt("##endFrame", &endFrame, ImGuiInputTextFlags_None);
       }
       
    }

    return 0;
}