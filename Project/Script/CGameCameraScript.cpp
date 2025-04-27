#include "pch.h"
#include "CGameCameraScript.h"

CGameCameraScript::CGameCameraScript()
	: CScript((UINT)SCRIPT_TYPE::GAMECAMERASCRIPT)
	, m_pTarget(nullptr)
    , m_fMoveTime(0.f)
    , m_fPrevMoveTime(0.f)
    , m_fDiffer(0.f)
    , m_fTargetScale(0.f)
    , m_fPrevScale(0.f)
	, m_vDistance(0.f, 2000.f, 2000.f)
	, m_vOffset{}
	, m_fAccTime(0.f)
	, m_fMaxTime(0.f)
	, m_fRange(0.f)
	, m_fShakeSpeed(0.f)
	, m_fShakeDir(0.f)
	, m_bCamShake(false)
	, m_bCutSceneView(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fMoveTime, "MoveTime");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fPrevMoveTime, "PrevMoveTime");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDiffer, "Differ");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fTargetScale, "TargetScale");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fPrevScale, "PrevScale");
	AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDistance, "Distance");
}

CGameCameraScript::~CGameCameraScript()
{
}

void CGameCameraScript::begin()
{
	if (nullptr == m_pTarget)
		m_pTarget = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	Transform()->SetRelativeRot(XM_PI / 4.f, 0.f, 0.f);
	Camera()->SetScale(0.6f);
}

void CGameCameraScript::tick()
{
	if (m_fMoveTime != 0.f)
	{
		m_fMoveTime -= DT;
		float CurScale = Camera()->GetScale();
		CurScale + (m_fDiffer * DT);
		Camera()->SetScale(CurScale);

		if (m_fMoveTime <= 0.f && m_fPrevMoveTime != 0.f)
		{
			m_fMoveTime = m_fPrevMoveTime;
			m_fPrevMoveTime = 0.f;
			m_fDiffer = m_fPrevScale - CurScale / m_fMoveTime;
		}
	}
	
	else
	{
		// CutScene이 아닌 경우. 타겟의 머리위에서 찍음.
		if (!m_bCutSceneView)
		{
			Vec3 CurTargetPos = m_pTarget->Transform()->GetWorldPos();
			CurTargetPos.x += m_vDistance.x;
			CurTargetPos.y += m_vDistance.y;
			CurTargetPos.z -= m_vDistance.z;
			Transform()->SetRelativePos(CurTargetPos + m_vOffset);
			Transform()->SetRelativeRot(XM_PI / 4.f, 0.f, 0.f);
		}
		
		// CutScene인 경우. 타겟을 정면에서 찍음. 회전된 곳의 방향으로 일정 거리만큼 이동하고, 카메라의 방향은 반대 방향으로 줘야 함. 
		else
		{
			Vec3 CurTargetPos = m_pTarget->Transform()->GetWorldPos();
			Vec3 CurTargetScale = m_pTarget->Transform()->GetRelativeScale();
			Vec3 CurTargetXZDir = m_pTarget->Transform()->GetXZDir();

			CurTargetPos.x += CurTargetXZDir.x * 600.f;
			CurTargetPos.y += 100.f;
			CurTargetPos.z += CurTargetXZDir.z * 600.f;
			Transform()->SetRelativePos(CurTargetPos + m_vOffset);

			Vec3 CurTargetRot;
			CurTargetRot.y = XM_2PI - m_pTarget->Transform()->GetRelativeRot().y;

			Transform()->SetRelativeRot(CurTargetRot);
		}
	}

	ShackCamera();
}

void CGameCameraScript::BeginOverlap(CCollider3D* _Other)
{
}

void CGameCameraScript::EndOverlap(CCollider3D* _Other)
{
}

void CGameCameraScript::SetMoveCamera(float _vTargetScale, float _fTime)
{
	m_fPrevScale = Camera()->GetScale();
	m_fDiffer = Camera()->GetScale() - _vTargetScale;
	m_fMoveTime = _fTime;
	m_fPrevMoveTime = _fTime;
	m_fDiffer /= m_fMoveTime;
}

void CGameCameraScript::CameraShake(float _fRange, float _fShackSpeed, float _fTerm)
{
	m_fAccTime = 0.f;
	m_fMaxTime = _fTerm;
	m_fRange = _fRange;
	m_fShakeSpeed = _fShackSpeed;
	m_fShakeDir = 1.f;
	m_bCamShake = true;
}

void CGameCameraScript::ShackCamera()
{
	if (!m_bCamShake)
		return;

	m_fAccTime += DT;

	if (m_fMaxTime <= m_fAccTime)
	{
		m_bCamShake = false;
		m_vOffset = Vec2(0.f, 0.f);
	}

	m_vOffset.x += DT * m_fShakeSpeed * m_fShakeDir;
	m_fShakeSpeed -= m_fShakeSpeed * m_fMaxTime * DT;
	if (m_fRange < fabsf(m_vOffset.x))
	{
		m_vOffset.x = m_fRange * m_fShakeDir;
		m_fShakeDir *= -1;
	}
}

void CGameCameraScript::SetCutSceneView(bool _bCutSceneView)
{
	m_bCutSceneView = _bCutSceneView;

	// CutScene 모드 일때는 PERSPECTIVE로.
	if (_bCutSceneView)
	{
		Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	}

	// Normal 모드 일때는 ORTHOGRAPHIC로.
	else
	{
		Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}
}
