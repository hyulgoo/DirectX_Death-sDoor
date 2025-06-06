#pragma once


// 오브젝트 생성
class CGameObject;
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx);
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName);

// 오브젝트 삭제
void DestroyObject(CGameObject* _DeletObject);


// DrawDebugShape
void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugCube(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugCube(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugSphere(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugFrustum(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

Vec3 DecomposeRotMat(const Matrix& _matRot);

float GetDistance(Vec3 _Vec1, Vec3 _Vec2);


// Target의 prevpos와 worldpos를 넣으면 이동하는 각도를 알려줌.
// z축은 y로 계산하여 방향을 알려줌.
float GetDir(Vec3 _vStart, Vec3 _vTarget, bool _degree = false);
const float& GetSmoothDir(CGameObject* _pStartObject, CGameObject* _pTargetObj, const float& _fdegree = 2.3f);
const float& GetSmoothDir(Vec3 _vStart, Vec3 _vTarget, Vec3 _vPrevDir, float _degree = 2.3f);

// Player만 사용하는 함수
void AddForceCentertoMouseDir(CGameObject* _pProjectile);



const char* ToString(RES_TYPE);
const wchar_t* ToWSTring(RES_TYPE);

const char* ToString(COMPONENT_TYPE);
const wchar_t* ToWSTring(COMPONENT_TYPE);


// Relative Path 가져오기
wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath);


// FbxMatrix -> Matrix
Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat);


// DXGI_FORMAT -> Size(Byte)
int GetSizeofFormat(DXGI_FORMAT _eFormat);


// Save / Load
void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

class CRes;
template<typename T>
class Ptr;

void SaveResRef(Ptr<CRes> _Res, FILE* _File);

class CResMgr;
template<typename T>
void LoadResRef(Ptr<T>& _Res, FILE* _File)
{
	int i = 0;	
	fread(&i, sizeof(int), 1, _File);
	
	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		
		LoadWString(strRelativePath, _File);

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}









template<typename T, UINT Size>
void Safe_Del_Array(T* (&arr)[Size])
{
	for (UINT i = 0; i < Size; ++i)
	{
		if (nullptr != arr[i])
			delete arr[i];
	}	
}

template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
		{
			delete _vec[i];
		}
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	_map.clear();
}



