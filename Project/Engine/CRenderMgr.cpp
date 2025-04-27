#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CCamera.h"
#include "CLight2D.h"
#include "CLight3D.h"

#include "CResMgr.h"
#include "CMRT.h"

CRenderMgr::CRenderMgr()
    : m_Light2DBuffer(nullptr)
    , RENDER_FUNC(nullptr)
    , m_pEditorCam(nullptr)
{
    Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
    m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
                                                    , (UINT)vResolution.x, (UINT)vResolution.y
                                                    , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
                                                    , D3D11_USAGE_DEFAULT);

    CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl")->SetTexParam(TEX_0, m_RTCopyTex);
    CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl")->SetTexParam(TEX_0, m_RTCopyTex);
    CResMgr::GetInst()->FindRes<CMaterial>(L"WindMtrl")->SetTexParam(TEX_0, m_RTCopyTex);
    Ptr<CTexture> NoiseTextue = CResMgr::GetInst()->Load<CTexture>(L"texture\\Deaths_Door\\noise.png", L"texture\\Deaths_Door\\noise.png"); 
    CResMgr::GetInst()->FindRes<CMaterial>(L"WindMtrl")->SetTexParam(TEX_1, NoiseTextue.Get());
    Vec2 NoiseTextureSize(NoiseTextue->Width(), NoiseTextue->Height());
    CResMgr::GetInst()->FindRes<CMaterial>(L"WindMtrl")->SetScalarParam(VEC2_0, &NoiseTextureSize);
}

CRenderMgr::~CRenderMgr()
{
    if (nullptr != m_Light2DBuffer)
        delete m_Light2DBuffer;

    if (nullptr != m_Light3DBuffer)
        delete m_Light3DBuffer;

    Safe_Del_Array(m_MRT);
}

void CRenderMgr::render_clear()
{
    MRT_Clear();

    // Texture Register Clear
    for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
    {
        CTexture::Clear(i);
    }    
}

void CRenderMgr::render()
{
    // 출력 대상이 Device가 갖고있는 SwapChain의 RT가 아닌 MRT의 RT가 됐으므로
    // MRT에서 Clear 및 OMSet을 하도록 함.
    // 렌더링 시작
    render_clear();

    // 광원 및 전역 데이터 업데이트 및 바인딩
    UpdateData();

    // 렌더 함수 호출
    (this->*RENDER_FUNC)();
    
    // 광원 해제
    Clear();
}




void CRenderMgr::render_play()
{
    // 전역광원 시점에서 shadow맵핑을 위한 DepthMap 생성
    render_dynamic_shadowdepth();

    // 카메라 기준 렌더링
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        if (nullptr == m_vecCam[i])
            continue;

        m_vecCam[i]->SortObject();
        m_vecCam[i]->render();
    }
}

void CRenderMgr::render_editor()
{
    // Directional 광원 시점에서 Shadow맵핑을 위한 DepthMap 생성
    render_dynamic_shadowdepth();

    m_pEditorCam->SortObject();
    m_pEditorCam->render();    
}

void CRenderMgr::render_dynamic_shadowdepth()
{
    m_MRT[(UINT)MRT_TYPE::SHADOW]->OMSet();

    for (size_t i = 0; i < m_vecLight3D.size(); ++i)
    {
        if (LIGHT_TYPE::DIRECTIONAL == m_vecLight3D[i]->GetLightType())
            m_vecLight3D[i]->render_depthmap();
    }
}




int CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
    if (m_vecCam.size() <= _idx)
    {
        m_vecCam.resize(_idx + 1);
    }

    m_vecCam[_idx] = _Cam;    
    return _idx;
}

void CRenderMgr::SetRenderFunc(bool _IsPlay)
{
    if(_IsPlay)
        RENDER_FUNC = &CRenderMgr::render_play;
    else
        RENDER_FUNC = &CRenderMgr::render_editor;
}

CCamera* CRenderMgr::GetMainCam()
{
    if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
    {
        if (m_vecCam.empty())
            return nullptr;
        return m_vecCam[0];
    }
    else
        return m_pEditorCam;
}

void CRenderMgr::CopyRenderTarget()
{
    Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::MRT_Clear()
{
    for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
    {
        if (i == (UINT)MRT_TYPE::WATER)
            continue;
        m_MRT[i]->Clear();
    }
}

void CRenderMgr::MRT_Clear(MRT_TYPE _Type)
{
    m_MRT[(UINT)_Type]->Clear();
}

void CRenderMgr::UpdateData()
{
    // GlobalData 에 광원 개수정보 세팅
    GlobalData.Light2DCount = (UINT)m_vecLight2D.size();
    GlobalData.Light3DCount = (UINT)m_vecLight3D.size();

    // 구조화버퍼의 크기가 모자라면 더 크게 새로 만든다.
    if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
    {
        m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight2D.size(), SB_TYPE::READ_ONLY, true);
    }

    static vector<tLightInfo> vecLight2DInfo;
    vecLight2DInfo.clear();

    for (size_t i = 0; i < m_vecLight2D.size(); ++i)
    {
        vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
    }

    // 구조화버퍼로 광원 데이터를 옮긴다.
    m_Light2DBuffer->SetData(vecLight2DInfo.data(), sizeof(tLightInfo) * (UINT)vecLight2DInfo.size());
    m_Light2DBuffer->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);

    // 구조화버퍼의 크기가 모자라면 더 크게 새로 만든다.
    if (m_Light3DBuffer->GetElementCount() < m_vecLight3D.size())
    {
        m_Light3DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight3D.size(), SB_TYPE::READ_ONLY, true);
    }

    static vector<tLightInfo> vecLight3DInfo;
    vecLight3DInfo.clear();

    for (size_t i = 0; i < m_vecLight3D.size(); ++i)
    {
        vecLight3DInfo.push_back(m_vecLight3D[i]->GetLightInfo());
    }

    // 구조화버퍼로 광원 데이터를 옮긴다.
    m_Light3DBuffer->SetData(vecLight3DInfo.data(), sizeof(tLightInfo) * (UINT)vecLight3DInfo.size());
    m_Light3DBuffer->UpdateData(13, PIPELINE_STAGE::PS_PIXEL);

    // 전역 상수 데이터 바인딩
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
    pGlobalBuffer->SetData(&GlobalData, sizeof(tGlobal));
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();
}


void CRenderMgr::Clear()
{
    m_vecLight2D.clear();
    m_vecLight3D.clear();
}
