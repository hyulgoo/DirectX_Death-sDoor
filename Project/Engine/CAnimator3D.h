#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"

class CStructuredBuffer;

class CAnimator3D :
    public CComponent
{
private:
    const vector<tMTBone>*      m_pVecBones;
    const vector<tMTAnimClip>*  m_pVecClip;

    vector<float>				m_vecClipUpdateTime;
    vector<Matrix>				m_vecFinalBoneMat;      // 텍스쳐에 전달할 최종 행렬정보
    int							m_iFrameCount;          // 60
    double						m_dCurTime;
    int							m_iCurClip;             // 클립 인덱스	

    int							m_iFrameIdx;            // 클립의 현재 프레임
    int							m_iNextFrameIdx;        // 클립의 다음 프레임
    float						m_fRatio;	            // 프레임 사이 비율

    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
    bool						m_bFinalMatUpdate;      // 최종행렬 연산 수행여부
    bool                        m_bRepeat;              // 반복 여부. 반복인 경우에는 m_bCurClipFinish를 true로 바꾸지 않는다.
    bool                        m_bCurClipFinish;       // 현재 애니메이션이 끝났는지에 대한 여부.
    bool                        m_bStop;

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    double GetCurClipTimeLength() { return m_pVecClip->at(m_iCurClip).dTimeLength; }

    bool IsStop() { return m_bStop; }
    void SetStop(bool _bStop) { m_bStop = _bStop; }

    void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);      // 애니메이션 클립 정보를 세팅하고, 애니메이션 별 재생 시간(현재 얼마나 재생되었는지)을 0으로 초기화 시킴.
    void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }
    void Play(int _iClipIdx, bool _bRepeat);                    // 현재 Clip을 변경하는 함수.
    int GetCurClip() { return m_iCurClip; }                     // 현재 Cilp 인덱스를 반환하는 함수.
    int GetClipSize() { return (int)m_vecClipUpdateTime.size(); }    // 전체 Clip 개수를 반환하는 함수.

    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    void ClearData();

    const vector<tMTAnimClip>* GetAnimClip() { return m_pVecClip; } // 현재 애니메이션 클립 정보 Vecotr를 반환하는 함수.
    void SaveMeshAnimationClip();                                   // 현재 Animator3D가 갖고 있는 m_pVecClip를 Mesh에 세팅하고, Mesh->Save()를 호출해서 클립 정보를 저장하는 함수. 애니메이션 클립은 Mesh에서 Save 및 Load를 한다. 

    // 임시
    int GetFrameCount() { return m_iFrameCount; }
    void SetFrameCount(int _FrameCount) { m_iFrameCount = _FrameCount; }
    bool IsFinish() { return m_bCurClipFinish; }

private:
    void check_mesh(Ptr<CMesh> _pMesh);

public:
    virtual void SaveToLevelFile(FILE* _pFile) override;
    virtual void LoadFromLevelFile(FILE* _pFile) override;
    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};
