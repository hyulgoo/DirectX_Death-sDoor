#include "pch.h"
#include "SoundUI.h"
#include <Engine/CSound.h>
#include <Engine/CResMgr.h>

SoundUI::SoundUI()
    : ResUI(RES_TYPE::SOUND),
    m_volume(0.5f)
{
    SetName("Sound");
}

SoundUI::~SoundUI()
{
}

int SoundUI::render_update()
{
    ResUI::render_update();
    //pChannel = CSound::g_pFMOD->getChannel(-1, &pChannel);
    Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(GetTargetRes()->GetRelativePath());
    

    //if (ImGui::Button("Play", ImVec2(100.f, 20.f)))
    //{
    //    idx = pSound->Play(0, m_volume);
    //}
    //ImGui::SameLine();
    //if (ImGui::Button("Resume", ImVec2(100.f, 20.f)))
    //{
    //    pSound->pause();
    //}
    //ImGui::SameLine();
    //if (ImGui::Button("Stop", ImVec2(100.f, 20.f)))
    //{
    //    pSound->Stop();
    //}
    //if (ImGui::Button("Volume Up", ImVec2(100.f, 20.f)))
    //{
    //    m_volume += 0.1f;
    //    pSound->SetVolume(m_volume , idx);
    //}
    //ImGui::SameLine();
    //if (ImGui::Button("Volume Down", ImVec2(100.f, 20.f)))
    //{
    //    m_volume -= 0.1f;
    //    pSound->SetVolume(m_volume, idx);
    //}
    //ImGui::DragFloat("##volume", &m_volume);
    ////

    return 0;
}