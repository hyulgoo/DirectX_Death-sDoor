#pragma once

class CLevel;
class CLayer;
class CGameObject;
class CPrefab;

class CLevelSaveLoad
{
public:
	static int SaveLevel(CLevel* _Level);
	static int Play(const wstring& _LevelPath, CLevel* _Level);
	static int SaveGameObject(CGameObject* _Object, FILE* _File);
	static int SavePrefab(const wstring& _strRelativePath, CPrefab* _Prefab);

	static CLevel* LoadLevel(LEVEL_STATE _state);
	static CLevel* Stop(const wstring& _LevelPath, LEVEL_STATE _state);
	static CGameObject* LoadGameObject(FILE* _File);
	static CGameObject* LoadPrefab(const wstring& _strRelativePath);


	//Prefab 생성
	static void SpawnPrefab(wstring _relativepath, int ind, Vec3 _vWorldPos, float time = -1);
	static CGameObject* SpawnandReturnPrefab(wstring _relativepath, int idx, Vec3 _vWorldPos, float time = -1);
};
