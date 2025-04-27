#include "pch.h"
#include "CDetourMgr.h"

#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CCamera.h"
#include "CTransform.h"
#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include <Detour\DetourNavMesh.h>
#include <Detour\DetourNavMeshBuilder.h>
#include <Detour\DetourNavMeshQuery.h>

#include <fstream>

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};


CDetourMgr::CDetourMgr()
	: m_pPlayer(nullptr)
	, m_pNaviMesh(nullptr)
{
}
CDetourMgr::~CDetourMgr()
{
	if (nullptr != m_pNaviMesh)
	{
		delete m_pNaviMesh;
	}
}

void CDetourMgr::init()
{
}

void CDetourMgr::ChangeLevel(LEVEL_TYPE _LevelType)
{
	if (nullptr != m_pNaviMesh)
		delete m_pNaviMesh;
	m_pNaviMesh = nullptr;
	m_pPlayer = nullptr;

	switch (_LevelType)
	{
	case LEVEL_TYPE::CASTLE_FIELD:
		LoadNavMeshFromBinFile("Navi\\Castle_Navi.bin");
		break;
	case LEVEL_TYPE::CASTLE_BOSS:
		LoadNavMeshFromBinFile("Navi\\Castle_Boss_Navi.bin");
		break;
	case LEVEL_TYPE::FOREST_FIELD:
		LoadNavMeshFromBinFile("Navi\\Forest_Navi.bin");
		break;
	case LEVEL_TYPE::ICE_FIELD:
		LoadNavMeshFromBinFile("Navi\\Ice.bin");
		break;
	case LEVEL_TYPE::ICE_BOSS:
		//LoadNavMeshFromBinFile("Navi\\Castle_Map.bin");
		break;
	case LEVEL_TYPE::HALL:
		LoadNavMeshFromBinFile("Navi\\Hall.bin");
		break;
	}
}

void CDetourMgr::LoadNavMeshFromBinFile(const char* path)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::wstring wpath = converter.from_bytes(path);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += wpath;

	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, strFilePath.c_str(), L"rb");
	if (err != 0 || !fp)
	{
		return;
	}
	// Read header.
	NavMeshSetHeader header;
	fread(&header, sizeof(NavMeshSetHeader), 1, fp);
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose(fp);
		return;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose(fp);
		return;
	}

	m_pNaviMesh = dtAllocNavMesh();
	if (!m_pNaviMesh)
	{
		fclose(fp);
		return;
	}

	dtStatus status = m_pNaviMesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		fclose(fp);
		return;
	}

	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		fread(&tileHeader, sizeof(tileHeader), 1, fp);
		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		fread(data, tileHeader.dataSize, 1, fp);

		m_pNaviMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}

	fclose(fp);
}

void CDetourMgr::RegisterPlayer(CGameObject* _pPlayer)
{
	if (nullptr == m_pPlayer)
		m_pPlayer = _pPlayer;
}

void CDetourMgr::DeletePlayer()
{
	if (nullptr != m_pPlayer)
		m_pPlayer = nullptr;
}

Vec3* CDetourMgr::GetPathtoTarget(Vec3 _vStartPos, int* ActualPathCount)
{	
	if (nullptr == m_pPlayer)
	{
		m_pPlayer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	}

	return GetPathtoTarget(_vStartPos, m_pPlayer->Transform()->GetWorldPos(), ActualPathCount);
}

Vec3* CDetourMgr::GetPathtoTarget(Vec3 _vStartPos, Vec3 _vTargetPos, int* ActualPathCount)
{
	if (nullptr == m_pNaviMesh)
		assert(nullptr);

	float actualPath[256 * 3] = { 0.f, };
	for (int i = 0; i < 256 * 3; i++)
		actualPath[i] = 0.0f;

	// NavMesh와 함께 경로 계획을 수행하는 Query 객체 생성
	dtNavMeshQuery* navQuery = dtAllocNavMeshQuery();
	navQuery->init(m_pNaviMesh, 4866);

	// 시작, 도착 위치 설정
	float startpos[3] = {};
	startpos[0] = _vStartPos.x;
	startpos[1] = _vStartPos.y;
	startpos[2] = -_vStartPos.z;

	float endpos[3] = {};
	Vec3 vEndPos = _vTargetPos;
	endpos[0] = vEndPos.x;
	endpos[1] = vEndPos.y;
	endpos[2] = -vEndPos.z;

	dtPolyRef startRef, endRef;
	float polyPickExt[3] = { 30000,30000,30000 }; // 범위를 제한하기 위한 벡터

	dtQueryFilter filter;
	filter.setIncludeFlags(0xFFFF); // 길찾기에 사용될 모든 폴리곤 참조.
	filter.setExcludeFlags(0); // 폴리곤을 제외하지 않음.

	navQuery->findNearestPoly(startpos, polyPickExt, &filter, &startRef, 0);
	navQuery->findNearestPoly(endpos, polyPickExt, &filter, &endRef, 0);

	// 시작과 도착 위치까지 경로 탐색
	float nearestStartPos[3], nearestEndPos[3];
	navQuery->closestPointOnPoly(startRef, startpos, nearestStartPos, 0);
	navQuery->closestPointOnPoly(endRef, endpos, nearestEndPos, 0);

	// 경로를 계획.
	dtPolyRef path[256];
	int pathCount;
	navQuery->findPath(startRef, endRef, nearestStartPos, nearestEndPos, &filter, path, &pathCount, 256);

	// 경로를 따라 실제 이동 경로를 생성
	navQuery->findStraightPath(nearestStartPos, nearestEndPos, path, pathCount, actualPath, 0, 0, ActualPathCount, 256);

	// Query 객체 할당 해제
	dtFreeNavMeshQuery(navQuery);

	Vec3 Path[256] = {};
	for (int i = 0; i < 256; ++i)
	{
		Path[i] = Vec3(actualPath[i * 3], actualPath[i * 3 + 1], actualPath[i * 3 + 2]);
		if (Vec3(0.f, 0.f, 0.f) == Path[i])
			break;
	}

	return Path;
}

float CDetourMgr::GetDirtoTarget(Vec3 _vStartPos)
{
	Vec3 vPlayerPos = m_pPlayer->Transform()->GetWorldPos();
	
	return GetDir(vPlayerPos, _vStartPos);;
}

float CDetourMgr::GetSmoothDirtoTarget(CGameObject* _pStartObj, float _fdegree)
{
	return GetSmoothDir(_pStartObj, m_pPlayer, _fdegree);
}
