#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;
    Vec3    m_vPrevPos;

    bool    m_bAbsolute;    // 상대 이동, 크기를 절대값으로 지정    

    Vec3    m_vRelativeDir[3];
    Vec3    m_vWorldDir[3];

    Matrix  m_matWorldScale;    // 월드 크기 행렬
    Matrix	m_matRelativeRot;   // 월드 회전 행렬

    Matrix  m_matWorld;         // 크기, 회전, 이동 정보를 합쳐놓음
    Matrix  m_matWorldInv;      // 월드 역행렬

public:
    void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
    void SetRelativeScale(Vec3 _vScale) { m_vRelativeScale = _vScale; }
    void SetRelativeRot(Vec3 _vRot) { m_vRelativeRot = _vRot; }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z);  }

    // 상대 이동, 크기를 절대값으로 지정  
    bool IsAbsolute() { return m_bAbsolute; }
    void SetAbsolute(bool _Set) { m_bAbsolute = _Set; }    

    const Vec3& GetRelativePos() const { return m_vRelativePos; }
    const Vec3& GetRelativeScale() const { return m_vRelativeScale; }
    const Vec3& GetRelativeRot() const { return m_vRelativeRot; }
    const Vec3& GetXZDir()
    {
        Vec3 WorldDir = m_vWorldDir[(UINT)DIR_TYPE::FRONT];
        WorldDir.y = 0.f;
        WorldDir.Normalize();
        return WorldDir;
    }
    Vec3 GetRelativeDir(DIR_TYPE _type) const { return m_vRelativeDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { { return m_vWorldDir[(UINT)_type]; } }
    const Vec3& GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetPrevPos() { return m_vPrevPos; }

    Matrix GetWorldRotation();

    const Matrix& GetWorldScaleMat() { return m_matWorldScale; }
    const Matrix& GetWorldMat() const { return m_matWorld; }
    const Matrix& GetWorldInvMat() const { return m_matWorldInv; }

    void SetWorldMat(const Matrix& _mat) { m_matWorld = _mat; }

public:
    virtual void finaltick() override;    
    void UpdateData();
    void CalcDir();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CTransform);
public:
    CTransform();
    ~CTransform();
};

