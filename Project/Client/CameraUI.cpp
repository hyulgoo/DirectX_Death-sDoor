#include "pch.h"
#include "CameraUI.h"

#include <Engine/CTransform.h>
#include <Engine/CCamera.h>

CameraUI::CameraUI()
    : ComponentUI("##Camera", COMPONENT_TYPE::CAMERA)
{
    SetName("Camera");
}

CameraUI::~CameraUI()
{
}

int CameraUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    // Camera Type
    ImGui::Text("PROJ_TYPE");
    ImGui::SameLine();
    int iProj_Type = (int)GetTarget()->Camera()->GetProjType();
    const char* Proj_Type[2] = { "ORTHOGRAPHIC",  "PERSPECTIVE" };    
    if (ImGui::Combo("##CollidereD Type", &iProj_Type, Proj_Type, 2, 2))
    {
        GetTarget()->Camera()->SetProjType((PROJ_TYPE)iProj_Type);
    }

    // Orthographic에서만 사용하는 Scale 설정
    if (iProj_Type == (int)PROJ_TYPE::ORTHOGRAPHIC)
    {
        float fScale = GetTarget()->Camera()->GetScale();
        ImGui::Text("SCALE    ");
        ImGui::SameLine();
        if (ImGui::SliderFloat("##Scale", &fScale, 0.f, 10.f))
        {
            GetTarget()->Camera()->SetScale(fScale);
        }
    }

    // Far
    float fFar = GetTarget()->Camera()->GetFar();
    ImGui::Text("FAR      ");
    ImGui::SameLine();
    if (ImGui::SliderFloat("##Far", &fFar, 1.1f, 10000.f))
    {
        GetTarget()->Camera()->SetFar(fFar);
    }

    static bool bDrawFrustum = false;
    ImGui::Text("DrawDubug");
    ImGui::SameLine();
    if(ImGui::Checkbox("##DrawFrustum", &bDrawFrustum))
    {
        GetTarget()->Camera()->SetDrawDebugShape(bDrawFrustum);
    }


    return TRUE;
}
