#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_RWBuffer(nullptr)
	, m_ModuleData{}
	, m_AccTime(0.f)
{

	//================
	// 의미없는 초기세팅
	//================
	m_ModuleData.iMaxParticleCount = 1000;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
	m_ModuleData.SpawnRate = 20;
	m_ModuleData.vSpawnColor = Vec3(0.4f, 1.f, 0.4f);
	m_ModuleData.vSpawnScaleMin = Vec3(15.f, 15.f, 1.f);
	m_ModuleData.vSpawnScaleMax = Vec3(20.f, 20.f, 1.f);

	//m_ModuleData.SpawnShapeType = 0;
	//m_ModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);	
	//m_ModuleData.Space = 0; // 시뮬레이션 좌표계

	//m_ModuleData.MinLifeTime = 3.f;
	//m_ModuleData.MaxLifeTime = 5.f;

	//m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = false;
	//m_ModuleData.StartScale = 1.5f;
	//m_ModuleData.EndScale = 0.2f;

	//m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	//m_ModuleData.vStartColor = Vec3(0.2f, 0.3f, 1.0f);
	//m_ModuleData.vEndColor = Vec3(0.4f, 1.f, 0.4f);

	//m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	//m_ModuleData.AddVelocityType = 0; // From Center
	//m_ModuleData.Speed = 700.f;
	//m_ModuleData.vVelocityDir;
	//m_ModuleData.OffsetAngle;

	//m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	//m_ModuleData.StartDrag = 500.f;
	//m_ModuleData.EndDrag = -500.f;

	//m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	//m_ModuleData.fNoiseTerm = 0.3f;
	//m_ModuleData.fNoiseForce = 50.f;

	//m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
	//m_ModuleData.VelocityAlignment = true;
	//m_ModuleData.VelocityScale = true;
	//m_ModuleData.vMaxVelocityScale = Vec3(15.f, 1.f, 1.f);
	//m_ModuleData.vMaxSpeed = 500.f;

	//m_Tex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\flame1.png");


	//========================================================

	//SetName(L"ParticleSystem");
	// 입자 메쉬
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// 파티클 전용 재질
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);

	// 파티클 업데이트 컴퓨트 쉐이더	
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);

	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _Other)
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ModuleData(_Other.m_ModuleData)
	, m_Tex(_Other.m_Tex)
{
	// 입자 메쉬
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// 파티클 전용 재질
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);

	// 파티클 업데이트 컴퓨트 쉐이더	
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);

	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_RWBuffer)
		delete m_RWBuffer;

	if (nullptr != m_ModuleDataBuffer)
		delete m_ModuleDataBuffer;
}


void CParticleSystem::finaltick()
{
	// 스폰 레이트 계산
	// 1개 스폰 시간
	float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
	m_AccTime += DT;

	// 누적시간이 개당 생성시간을 넘어서면
	if (fTimePerCount < m_AccTime)
	{
		// 초과 배율 ==> 생성 개수
		float fData = m_AccTime / fTimePerCount;

		// 나머지는 남은 시간
		m_AccTime = fTimePerCount * (fData - floor(fData));

		// 버퍼에 스폰 카운트 전달
		tRWParticleBuffer rwbuffer = { (int)fData, };
		m_RWBuffer->SetData(&rwbuffer);
	}


	// 파티클 업데이트 컴퓨트 쉐이더
	m_ModuleDataBuffer->SetData(&m_ModuleData);

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetRWParticleBuffer(m_RWBuffer);
	m_UpdateCS->SetModuleData(m_ModuleDataBuffer);
	m_UpdateCS->SetNoiseTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_01.png"));
	m_UpdateCS->SetParticleObjectPos(Transform()->GetWorldPos());

	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	// 파티클버퍼 t20 에 바인딩
	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);

	// 모듈 데이터 t21 에 바인딩
	m_ModuleDataBuffer->UpdateData(21, PIPELINE_STAGE::PS_GEOMETRY);

	// Particle Render	
	//Ptr<CTexture> pParticleTex = CResMgr::GetInst()->Load<CTexture>(L"Particle_0", L"texture\\particle\\flame1.png");
	GetMaterial(0)->SetTexParam(TEX_0, m_Tex);

	GetMaterial(0)->UpdateData();
	GetMesh()->render_particle(m_ModuleData.iMaxParticleCount);

	// 파티클 버퍼 바인딩 해제
	m_ParticleBuffer->Clear();
	m_ModuleDataBuffer->Clear();
}

void CParticleSystem::SpawnModule(int _MaxParticle, int _SpawnRate, Vec3 _SpawnColor, Vec3 SpawnMinScale
	, Vec3 SpawnMaxScale, Vec3 _SpawnBoxScale, float _MinLifeTime, float _MaxLifeTime)
{
	for (UINT i = 0; i < (UINT)PARTICLE_MODULE::END; ++i)
	{
		m_ModuleData.ModuleCheck[i] = false;
	}
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
	m_ModuleData.iMaxParticleCount = _MaxParticle;
	m_ModuleData.SpawnRate = _SpawnRate;
	m_ModuleData.vSpawnColor = _SpawnColor / 255;
	m_ModuleData.vSpawnScaleMin = SpawnMinScale;
	m_ModuleData.vSpawnScaleMax = SpawnMaxScale;
	m_ModuleData.SpawnShapeType = 0;
	m_ModuleData.vBoxShapeScale = _SpawnBoxScale;
	m_ModuleData.Space = 0; // 시뮬레이션 좌표계
	m_ModuleData.MinLifeTime = _MinLifeTime;
	m_ModuleData.MaxLifeTime = _MaxLifeTime;

}

void CParticleSystem::ColorChangeModule(Vec3 _vStartColor, Vec3 _vEndColor)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	m_ModuleData.vStartColor = _vStartColor / 255.f;
	m_ModuleData.vEndColor = _vEndColor / 255;
}

void CParticleSystem::ScaleChangeModule(float _vStartScale, float _vEndScale)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	m_ModuleData.StartScale = _vStartScale;
	m_ModuleData.EndScale = _vEndScale;
}

void CParticleSystem::AddVelocityModule(float _fSpeed, int _iVelocityType, Vec3 _vVelocityDir, float _fAngle)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_ModuleData.AddVelocityType = _iVelocityType;
	m_ModuleData.Speed = _fSpeed;
	m_ModuleData.vVelocityDir = _vVelocityDir;
	m_ModuleData.OffsetAngle = _fAngle;
}

void CParticleSystem::DragModule(float _fStartDrag, float _fEndDrag)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	m_ModuleData.StartDrag = _fStartDrag;
	m_ModuleData.EndDrag = -_fEndDrag;
}

void CParticleSystem::RandomForceModule(float _fForceTerm, float _fForce)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = true;
	m_ModuleData.fNoiseTerm = _fForceTerm;
	m_ModuleData.fNoiseForce = _fForce;
}

void CParticleSystem::VelocityAlignmentModule()
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_ModuleData.VelocityAlignment = true;
}

void CParticleSystem::VelocityScaleModule(float _fMaxSpeed, Vec3 _vMaxVelocityScale)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_ModuleData.VelocityScale = true;
	m_ModuleData.vMaxVelocityScale = _vMaxVelocityScale;
	m_ModuleData.vMaxSpeed = _fMaxSpeed;
}

void CParticleSystem::AnimationModule(int _iFrmCount, int _iXCount, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vOffset)
{
	Vec2 Resolution = Vec2(m_Tex->Width(), m_Tex->Height());

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION] = true;
	m_ModuleData.iFrmCount = _iFrmCount;
	m_ModuleData.iXCount = _iXCount;
	m_ModuleData.vLeftTop = _vLeftTop / Resolution;
	m_ModuleData.vSlice = _vSlice / Resolution;
	m_ModuleData.vOffset = _vOffset / Resolution;
}

void CParticleSystem::OnOff(bool _Onff)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = _Onff;
	m_AccTime = 0.f;
}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	fwrite(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	SaveResRef(m_UpdateCS.Get(), _File);
	SaveResRef(m_Tex.Get(), _File);
}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_ModuleData, sizeof(tParticleModule), 1, _File);

	int i = 0;
	fread(&i, sizeof(i), 1, _File);

	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();

	}
	LoadResRef(m_Tex, _File);
	//m_Tex = CResMgr::GetInst()->FindRes<CTexture>(m_Tex);
}
