#pragma once

class CLevel;
class CLayer;
class CGameObject;
class CPrefab;

class CLevelSaveLoadInScript
{
public:
	static int SaveLevel(CLevel* _pLevel);
	static int Play(const wstring& _strLevelPath, CLevel* _pLevel);
	static int SaveGameObject(CGameObject* _pObject, FILE* _File);
	static int SavePrefab(const wstring& _strRelativePath, CPrefab* _Prefab);

	static CLevel* LoadLevel(LEVEL_STATE _tState);
	static CLevel* Stop(const wstring& _strLevelPath, LEVEL_STATE _tState);
	static CGameObject* LoadGameObject(FILE* _File);
	static CGameObject* LoadPrefab(const wstring& _strRelativePath);


	//Prefab 생성
	static void SpawnPrefab(wstring _strRelativepath, int _Idx, Vec3 _vWorldPos, float _fTime = -1.f);
	static CGameObject* SpawnandReturnPrefab(wstring _strRelativepath, int ind, Vec3 _vWorldPos, float _fTime = -1.f);

	//자리수 계산
	int GetDigitCount(int _iMoney);

	//데미지 출력
	void ShowMoney(int _iMoney, int _iDigitCount);
	void MoneyCount(int _iMoney) { ShowMoney(_iMoney, GetDigitCount(_iMoney)); }//이걸 사용하면 됨다.
	void AddChild(CGameObject* _pOwner, CGameObject* _pChild);
};

