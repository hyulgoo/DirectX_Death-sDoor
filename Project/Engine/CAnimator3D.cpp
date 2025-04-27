#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_iCurClip(0)
	, m_dCurTime(0.0)
	, m_iFrameCount(60)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_bRepeat(false)
	, m_bCurClipFinish(false)
	, m_bStop(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pVecBones(_origin.m_pVecBones)
	, m_pVecClip(_origin.m_pVecClip)
	, m_iCurClip(_origin.m_iCurClip)
	, m_dCurTime(_origin.m_dCurTime)
	, m_iFrameCount(_origin.m_iFrameCount)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_iFrameIdx(_origin.m_iFrameIdx)
	, m_iNextFrameIdx(_origin.m_iNextFrameIdx)
	, m_fRatio(_origin.m_fRatio)
	, m_bRepeat(false)
	, m_bCurClipFinish(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;
}


void CAnimator3D::finaltick()
{
	if (m_bStop)
		return;

	m_dCurTime = 0.f;

	// 현재 재생중인 Clip 의 시간을 진행한다.
	m_vecClipUpdateTime[m_iCurClip] += DT;

	// 현재 애니메이션 재생시간이 끝났으면, 다시 시작 부분으로 돌려서 반복 시킴.
	if (m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at(m_iCurClip).dTimeLength)
	{
		// 반복 재생이면 처음부터 다시 재생
		if (m_bRepeat)
			m_vecClipUpdateTime[m_iCurClip] = 0.f;
		
		// 1회 재생이면 
		else
			m_bCurClipFinish = true;
	}

	// 애니메이션 재생이 끝나지 않은 경우.
	if (!m_bCurClipFinish)
	{
		// 클립 내에서 현재 애니메이션 시작 시간 + 현재 애니메이션이 지금까지 재생된 시간 = 현재 클립 내에서의 시간.
		m_dCurTime = m_pVecClip->at(m_iCurClip).dStartTime + m_vecClipUpdateTime[m_iCurClip];

		// 현재 프레임 인덱스 구하기
		double dFrameIdx = m_dCurTime * (double)m_iFrameCount;
		m_iFrameIdx = (int)(dFrameIdx);

		// 다음 프레임 인덱스
		// 클립 1개만 쓰니까 0 넣는거고 거기서 맨 뒤 프레임이면. 
		if (m_iFrameIdx >= m_pVecClip->at(m_iCurClip).iEndFrame)
			m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
		else
			m_iNextFrameIdx = m_iFrameIdx + 1;

		// 프레임간의 시간에 따른 비율을 구해준다.
		m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);

		// 컴퓨트 쉐이더 연산여부
		m_bFinalMatUpdate = false;
	}
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}


void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_fRatio);

		pUpdateShader->SetBlendFrameIdx(m_iFrameIdx);
		pUpdateShader->SetBlendRatio(0.5f);

		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_bFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimator3D::Play(int _iClipIdx, bool _bRepeat)
{
	if (_iClipIdx >= 0 && _iClipIdx < m_pVecClip->size()) 
		m_iCurClip = _iClipIdx; 

	// 현재 재생하는 애니메이션 클립을 변경하면 해당 애니메이션의 재생시간을 0으로 초기화.
	m_vecClipUpdateTime[_iClipIdx] = 0.f;
	m_bCurClipFinish = false;
	m_bRepeat = _bRepeat;
}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::SaveMeshAnimationClip()
{
	Ptr<CMesh> pMesh = MeshRender()->GetMesh();

	pMesh->ChangeAnimClip(m_pVecClip);	// Mesh의 AnimClip을 현재 Animator3D의 AnimClip으로 변경.

	pMesh->Save(pMesh->GetKey());		// Mesh에는 Key 값으로 경로가 저장되어 있을 것이다. 
}

void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
	Ptr<CMesh> pMesh = MeshRender()->GetMesh();
	SaveResRef(pMesh.Get(), _pFile);
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
	Ptr<CMesh> pMesh = nullptr;
	LoadResRef(pMesh, _pFile);

	SetBones(pMesh->GetBones());
	SetAnimClip(pMesh->GetAnimClip());
}