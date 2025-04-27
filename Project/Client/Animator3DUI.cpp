#include "pch.h"
#include "Animator3DUI.h"

#include <Engine/CAnimator3D.h>


Animator3DUI::Animator3DUI()
    : ComponentUI("##Animator3D", COMPONENT_TYPE::ANIMATOR3D)
    , m_bShowAnimInfo(false)
    , m_bMakeAnimInfo(false)
{
    SetName("Animator3D");
}

Animator3DUI::~Animator3DUI()
{
}


int Animator3DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    // 애니메이션 Stop 기능 확인
    ImGui::Text("Animation Stop");
    ImGui::SameLine();
    bool AnimStop = GetTarget()->Animator3D()->IsStop();
    ImGui::Checkbox("##AnimStop", &AnimStop);
    GetTarget()->Animator3D()->SetStop(AnimStop);

    ImGui::Text("CurClip       ");
    ImGui::SameLine();
    int CurClip = GetTarget()->Animator3D()->GetCurClip();
    int ClipSize = GetTarget()->Animator3D()->GetClipSize();

    if(ImGui::SliderInt("##AnimClip", &CurClip, 0, ClipSize - 1))
        GetTarget()->Animator3D()->Play(CurClip, true);

    // AnimClip 정보 출력 여부.
    /*ImGui::Text("ShowAnimInfo  ");
    ImGui::SameLine();
    ImGui::Checkbox("##ShowAnimInfo", &m_bShowAnimInfo);

    if (m_bShowAnimInfo)*/
    {
        // 현재 Animator3D의 AnimClip 정보를 출력하자
        ImGui::Text("AnimationClipInfo");
        vector<tMTAnimClip> vecAnimClip = *(GetTarget()->Animator3D()->GetAnimClip());

        ImGui::SetNextItemWidth(100.f);
        ImGui::Text("        AnimName");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(32.f);
        ImGui::Text("      StartF");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(32.f);
        ImGui::Text("EndF");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(32.f);
        ImGui::Text("FrmLen");

        // 애니메이션 클립 이름, 시작 Frame, 끝 Frame, Frame 길이 정보 총 4가지만 출력한다. 
        for (int i = 0; i < vecAnimClip.size(); ++i)
        {
            ImGui::Text("#%d     ", i);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.f);
            string TempUIName = string("##AnimName") + std::to_string(i);
            string Temp = string(vecAnimClip[i].strAnimName.begin(), vecAnimClip[i].strAnimName.end());
            ImGui::InputText(TempUIName.c_str(), (char*)Temp.c_str(), 50, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            TempUIName = string("##StartFrame") + std::to_string(i);
            ImGui::InputInt(TempUIName.c_str(), &(vecAnimClip[i].iStartFrame), 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            TempUIName = string("##EndFrame") + std::to_string(i);
            ImGui::InputInt(TempUIName.c_str(), &(vecAnimClip[i].iEndFrame), 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            TempUIName = string("##FrameLength") + std::to_string(i);
            ImGui::InputInt(TempUIName.c_str(), &(vecAnimClip[i].iFrameLength), 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
            
            ImGui::SameLine();
            TempUIName = string("##Button") + std::to_string(i);
            if (ImGui::Button(TempUIName.c_str(), ImVec2(18.f, 18.f)))
            {
                GetTarget()->Animator3D()->Play(i, true);
            }
        }
    }

    // Clip을 새로 만들고 싶다면.
    ImGui::Text("MakeAnimInfo  ");
    ImGui::SameLine();
    ImGui::Checkbox("##MakeAnimInfo", &m_bMakeAnimInfo);

    if (m_bMakeAnimInfo)
    {
        // 임시 AnimClip을 추가하는 함수.
        ImGui::Text("AddAnimClip   ");
        ImGui::SameLine();
        static char TempAnimName[128] = {};
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputText("##AddAnimName", TempAnimName, 128);
        ImGui::SameLine();
        if (ImGui::Button("##AddAnimClip", ImVec2(18.f, 18.f)))
        {
            tMTAnimClip temp = {};
            string strTemp = string(TempAnimName);
            temp.strAnimName = wstring(strTemp.begin(), strTemp.end());
            m_vecAnimClip.push_back(temp);
        }

        if (m_vecAnimClip.size())
        {
            ImGui::SetNextItemWidth(100.f);
            ImGui::Text("        AnimName");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            ImGui::Text("      StartF");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            ImGui::Text("EndF");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            ImGui::Text("FrmLen");
        }

        // 애니메이션 클립 이름, 시작 Frame, 끝 Frame, Frame 길이 정보 총 4가지만 출력한다. 
        for (int i = 0; i < m_vecAnimClip.size(); ++i)
        {
            ImGui::Text("#%d     ", i);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100.f);
            string TempUIName = string("##N.AnimName") + std::to_string(i);
            string Temp = string(m_vecAnimClip[i].strAnimName.begin(), m_vecAnimClip[i].strAnimName.end());
            ImGui::InputText(TempUIName.c_str(), (char*)Temp.c_str(), 128, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            TempUIName = string("##N.StartFrame") + std::to_string(i);
            ImGui::InputInt(TempUIName.c_str(), &(m_vecAnimClip[i].iStartFrame), 0, 0);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            TempUIName = string("##N.EndFrame") + std::to_string(i);
            ImGui::InputInt(TempUIName.c_str(), &(m_vecAnimClip[i].iEndFrame), 0, 0);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(32.f);
            m_vecAnimClip[i].iFrameLength = m_vecAnimClip[i].iEndFrame - m_vecAnimClip[i].iStartFrame;
            TempUIName = string("##N.FrameLength") + std::to_string(i);
            ImGui::InputInt(TempUIName.c_str(), &(m_vecAnimClip[i].iFrameLength), 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

            m_vecAnimClip[i].dStartTime = m_vecAnimClip[i].iStartFrame / 60.f;
            m_vecAnimClip[i].dEndTime = m_vecAnimClip[i].iEndFrame / 60.f;
            m_vecAnimClip[i].dTimeLength = m_vecAnimClip[i].iFrameLength / 60.f;
        }

        // AnimClip Mesh 파일에 저장.
        ImGui::Text("Save AnimClip ");
        ImGui::SameLine();
        if (ImGui::Button("##Save AnimClip", ImVec2(18.f, 18.f)))
        {
            GetTarget()->Animator3D()->SaveMeshAnimationClip();
        }
    }

    // Animator3D에 AnimClip 적용.
    ImGui::Text("Apply AnimClip");
    ImGui::SameLine();
    if (ImGui::Button("##Apply AnimClip", ImVec2(18.f, 18.f)))
    {
        GetTarget()->Animator3D()->SetAnimClip(&m_vecAnimClip);
    }

    // 임시. FrameCount 조절
    int FrameCount = GetTarget()->Animator3D()->GetFrameCount();
    ImGui::DragInt("FrameCount", &FrameCount);
    GetTarget()->Animator3D()->SetFrameCount(FrameCount);

    return TRUE;
}
