#include "pch.h"
#include "CLevelSaveLoadInScript.h"

#include <Engine\CPathMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CScript.h>
#include <Engine/CEventMgr.h>
#include <Engine/CPhysXMgr.h>
#include "CPlayerScript.h"
#include "commdlg.h"
#include <Engine/CPhysXMgr.h>

#include "CScriptMgr.h"
#include "CSpawnMgr.h"


int CLevelSaveLoadInScript::Play(const wstring& _strLevelPath, CLevel* _pLevel)
{
    if (_pLevel->GetState() != LEVEL_STATE::STOP)
        return E_FAIL;

    wstring strPath = CPathMgr::GetInst()->GetContentPath();
    strPath += _strLevelPath;

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    // 레벨 이름 저장
    SaveWString(_pLevel->GetName(), pFile);

    int level_type = _pLevel->GetLevelType();
    fwrite(&level_type, sizeof(int), 1, pFile);

    // 레벨의 레이어들을 저장
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        CLayer* pLayer = _pLevel->GetLayer(i);

        // 레이어 이름 저장
        SaveWString(pLayer->GetName(), pFile);

        // 레이어의 게임오브젝트들 저장
        const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

        // 오브젝트 개수 저장
        size_t objCount = vecParent.size();
        fwrite(&objCount, sizeof(size_t), 1, pFile);

        // 각 게임오브젝트
        for (size_t i = 0; i < objCount; ++i)
        {
            SaveGameObject(vecParent[i], pFile);
        }
    }

    fclose(pFile);


    return S_OK;
}

CLevel* CLevelSaveLoadInScript::Stop(const wstring& _LevelPath, LEVEL_STATE _tState)
{
    CPhysXMgr::GetInst()->Clear();
    CSpawnMgr::GetInst()->Clear();

    wstring strPath = CPathMgr::GetInst()->GetContentPath();
    strPath += _LevelPath;//상대경로

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"rb");

    if (nullptr == pFile)
        return nullptr;

    CLevel* pNewLevel = new CLevel;

    // 레벨 이름
    wstring strLevelName;
    LoadWString(strLevelName, pFile);
    pNewLevel->SetName(strLevelName);

    int iLevel_type = 0;
    fread(&iLevel_type, sizeof(int), 1, pFile);
    pNewLevel->SetLevelType(iLevel_type);

    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        CLayer* pLayer = pNewLevel->GetLayer(i);

        // 레이어 이름
        wstring LayerName;
        LoadWString(LayerName, pFile);
        pLayer->SetName(LayerName);

        // 게임 오브젝트 개수
        size_t ObjCount = 0;
        fread(&ObjCount, sizeof(size_t), 1, pFile);

        // 각 게임오브젝트
        for (size_t j = 0; j < ObjCount; ++j)
        {
            CGameObject* pNewObj = LoadGameObject(pFile);
            pNewLevel->AddGameObject(pNewObj, i, false);
        }
    }

    fclose(pFile);

    pNewLevel->ChangeState(_tState);

    return pNewLevel;
}

int CLevelSaveLoadInScript::SaveLevel(CLevel* _pLevel)
{
    if (_pLevel->GetState() != LEVEL_STATE::STOP) //stop상태일 때만 저장
        return E_FAIL;

    OPENFILENAME ofn = {};
    wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
    strFolderpath += L"Level\\";

    wchar_t szFilePath[256] = {};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFilePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 256;
    ofn.lpstrFilter = L"Level\0*.lv\0ALL\0*.*";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = strFolderpath.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (false == GetSaveFileName(&ofn))
        E_FAIL;

    // 파일 입출력
    FILE* pFile = nullptr;
    errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    // 레벨 이름 저장
    SaveWString(_pLevel->GetName(), pFile);

    int iLevel_type = _pLevel->GetLevelType();
    fwrite(&iLevel_type, sizeof(int), 1, pFile);

    // 레벨의 레이어들을 저장
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        CLayer* pLayer = _pLevel->GetLayer(i);

        // 레이어 이름 저장
        SaveWString(pLayer->GetName(), pFile);

        // 레이어의 부모게임오브젝트들 저장
        const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

        size_t ObjCount = vecParent.size();
        fwrite(&ObjCount, sizeof(size_t), 1, pFile); // 오브젝트 개수 저장

        for (size_t i = 0; i < ObjCount; ++i)
        {
            SaveGameObject(vecParent[i], pFile); // 각 게임오브젝트 저장
        }
    }

    fclose(pFile);


    return S_OK;
}

int CLevelSaveLoadInScript::SaveGameObject(CGameObject* _pObject, FILE* _File)
{
    // 이름
    SaveWString(_pObject->GetName(), _File);

    // 컴포넌트
    for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (i == (UINT)COMPONENT_TYPE::END)
        {
            // 컴포넌트 타입 저장
            fwrite(&i, sizeof(UINT), 1, _File);
            break;
        }

        CComponent* Com = _pObject->GetComponent((COMPONENT_TYPE)i);
        if (nullptr == Com)
            continue;

        // 컴포넌트 타입 저장
        fwrite(&i, sizeof(UINT), 1, _File);

        // 컴포넌트 정보 저장
        Com->SaveToLevelFile(_File);
    }

    // 스크립트   
    const vector<CScript*>& vecScript = _pObject->GetScripts();
    size_t ScriptCount = vecScript.size();
    fwrite(&ScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < vecScript.size(); ++i)
    {
        wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
        SaveWString(ScriptName, _File);
        vecScript[i]->SaveToLevelFile(_File);
    }


    // 자식 오브젝트
    const vector<CGameObject*>& vecChild = _pObject->GetChild();
    size_t ChildCount = vecChild.size();
    fwrite(&ChildCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ChildCount; ++i)
    {
        SaveGameObject(vecChild[i], _File);
    }

    return 0;
}

CLevel* CLevelSaveLoadInScript::LoadLevel(LEVEL_STATE _tState)
{
    OPENFILENAME ofn = {};
    wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
    strFolderpath += L"Level\\";

    wchar_t szFilePath[256] = {};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFilePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 256;
    ofn.lpstrFilter = L"Level\0*.lv\0ALL\0*.*";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = strFolderpath.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (false == GetSaveFileName(&ofn))
        E_FAIL;

    // 파일 입출력
    FILE* pFile = nullptr;
    errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"rb");

    if (nullptr == pFile)
        return nullptr;

    CLevel* pNewLevel = new CLevel;

    // 레벨 이름
    wstring strLevelName;
    LoadWString(strLevelName, pFile);
    pNewLevel->SetName(strLevelName);

    int iLevel_type = 0;
    fread(&iLevel_type, sizeof(int), 1, pFile);
    pNewLevel->SetLevelType(iLevel_type);

    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        CLayer* pLayer = pNewLevel->GetLayer(i);

        // 레이어 이름
        wstring LayerName;
        LoadWString(LayerName, pFile);
        pLayer->SetName(LayerName);

        // 게임 오브젝트 개수
        size_t objCount = 0;
        fread(&objCount, sizeof(size_t), 1, pFile);

        // 각 게임오브젝트
        for (size_t j = 0; j < objCount; ++j)
        {
            CGameObject* pNewObj = LoadGameObject(pFile);
            pNewLevel->AddGameObject(pNewObj, i, false);
        }
    }

    fclose(pFile);

    pNewLevel->ChangeState(_tState);

    return pNewLevel;
}

CGameObject* CLevelSaveLoadInScript::LoadGameObject(FILE* _File)
{
    CGameObject* pObject = new CGameObject;

    // 이름
    wstring Name;
    LoadWString(Name, _File);
    pObject->SetName(Name);

    // 컴포넌트
    while (true)
    {
        UINT ComponentType = 0;
        fread(&ComponentType, sizeof(UINT), 1, _File);

        // 컴포넌트 정보의 끝을 확인
        if ((UINT)COMPONENT_TYPE::END == ComponentType)
            break;

        CComponent* Component = nullptr;

        switch ((COMPONENT_TYPE)ComponentType)
        {
        case COMPONENT_TYPE::TRANSFORM:
            Component = new CTransform;
            break;
        case COMPONENT_TYPE::COLLIDER2D:
            Component = new CCollider2D;
            break;
        case COMPONENT_TYPE::COLLIDER3D:
            Component = new CCollider3D;
            break;
        case COMPONENT_TYPE::ANIMATOR2D:
            Component = new CAnimator2D;
            break;
        case COMPONENT_TYPE::ANIMATOR3D:
            Component = new CAnimator3D;
            break;
        case COMPONENT_TYPE::LIGHT2D:
            Component = new CLight2D;
            break;
        case COMPONENT_TYPE::LIGHT3D:
            Component = new CLight3D;
            break;
        case COMPONENT_TYPE::CAMERA:
            Component = new CCamera;
            break;
        case COMPONENT_TYPE::MESHRENDER:
            Component = new CMeshRender;
            break;
        case COMPONENT_TYPE::PARTICLESYSTEM:
            Component = new CParticleSystem;
            break;
        case COMPONENT_TYPE::TILEMAP:
            Component = new CTileMap;
            break;
        case COMPONENT_TYPE::SKYBOX:
            Component = new CSkyBox;
            break;
        case COMPONENT_TYPE::LANDSCAPE:
            Component = new CLandScape;
            break;
        case COMPONENT_TYPE::DECAL:
            Component = new CDecal;
            break;
        case COMPONENT_TYPE::RIGIDBODY:
            Component = new CRigidbody;
            break;
        }

        Component->LoadFromLevelFile(_File);
        pObject->AddComponent(Component);

        if (COMPONENT_TYPE::RIGIDBODY == Component->GetType())
        {
            CRigidbody* RigidbodyComponent = (CRigidbody*)Component;
            physx::PxGeometryType::Enum Type = RigidbodyComponent->GetShapeType();
            Vec3 vSpawnPos = RigidbodyComponent->SetSpawnPos();
            Vec3 vRigidScale = RigidbodyComponent->GetRigidScale();
            switch (Type)
            {
            case physx::PxGeometryType::Enum::eBOX:
                CPhysXMgr::GetInst()->CreateCube(vSpawnPos, vRigidScale, pObject);
                break;
            case physx::PxGeometryType::Enum::eCAPSULE:
                CPhysXMgr::GetInst()->CreateCapsule(vSpawnPos, vRigidScale.x, vRigidScale.y, pObject);
                break;
            case physx::PxGeometryType::Enum::eSPHERE:
                CPhysXMgr::GetInst()->CreateSphere(vSpawnPos, vRigidScale.x, pObject);
                break;
            }
        }
    }


    // 스크립트   
    size_t ScriptCount = 0;
    fread(&ScriptCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ScriptCount; ++i)
    {
        wstring ScriptName;
        LoadWString(ScriptName, _File);
        CScript* pScript = CScriptMgr::GetScript(ScriptName);
        pObject->AddComponent(pScript);
        pScript->LoadFromLevelFile(_File);
    }

    // 자식 오브젝트      
    size_t ChildCount = 0;
    fread(&ChildCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < ChildCount; ++i)
    {
        CGameObject* ChildObject = LoadGameObject(_File);
        pObject->AddChild(ChildObject);
    }

    return pObject;
}

int CLevelSaveLoadInScript::SavePrefab(const wstring& _strRelativePath, CPrefab* _Prefab)
{
    if (_Prefab->IsEngineRes())
        return E_FAIL;

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    SaveGameObject(_Prefab->GetProtoObj(), pFile);

    fclose(pFile);

    return S_OK;
}

CGameObject* CLevelSaveLoadInScript::LoadPrefab(const wstring& _strRelativePath)
{
    Ptr<CPrefab> pPrefab = new CPrefab;

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += _strRelativePath;

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

    CGameObject* pNewObj = CLevelSaveLoadInScript::LoadGameObject(pFile);
    fclose(pFile);

    return pNewObj;
}

void CLevelSaveLoadInScript::SpawnPrefab(wstring _Relativepath, int _iIdx, Vec3 _vWorldPos, float _fTime)
{
    wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
    wstring relativepath = _Relativepath;
    strFolderpath += relativepath;

    FILE* pFile = nullptr;
    errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");

    CGameObject* NewGameObject = LoadGameObject(pFile);
    Vec3 prefpos = _vWorldPos;

    SpawnGameObject(NewGameObject, _vWorldPos, _iIdx);
    if (_fTime >= 0.f)
        NewGameObject->SetLifeSpan(_fTime);
    fclose(pFile);
}
CGameObject* CLevelSaveLoadInScript::SpawnandReturnPrefab(wstring _Relativepath, int _iIdx, Vec3 _vWorldPos, float _fTime)
{
    wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
    wstring relativepath = _Relativepath;
    strFolderpath += relativepath;

    FILE* pFile = nullptr;
    errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");

    CGameObject* newPrefab = LoadGameObject(pFile);
    Vec3 prefpos = _vWorldPos;

    SpawnGameObject(newPrefab, _vWorldPos, _iIdx);
    if (_fTime >= 0.f)
        newPrefab->SetLifeSpan(_fTime);
    fclose(pFile);
    return newPrefab;
}
int CLevelSaveLoadInScript::GetDigitCount(int Damage)
{
    int count = 0;
    while (Damage != 0)//자리수 구하기
    {
        Damage = Damage / 10;
        count++;
    }
    return count;
}

void CLevelSaveLoadInScript::ShowMoney(int Money, int DigitCount)
{
    Vec3 rot = (Vec3(0.f, 0.f, 0.f) / 180.f) * XM_PI;
    {
        CGameObject* mText = SpawnandReturnPrefab(L"prefab\\+.prefab", (int)LAYER::UI, Vec3(1520, -1000.f, 0.f)/3, 3.f);
        mText->SetName(L"+");
        mText->MeshRender()->GetDynamicMaterial(0);
        mText->Transform()->SetRelativeRot(rot);
        mText->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 0.f));
        mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_plus.png", L"texture\\HUD\\n_plus.png", 0));
        
    }
    {
        CGameObject* mText = SpawnandReturnPrefab(L"prefab\\MoneyIcon.prefab", (int)LAYER::UI, Vec3(1270.f, -1150.f, 0.f)/3, 3.f);
        mText->SetName(L"MoneyIcon");
        mText->MeshRender()->GetDynamicMaterial(0);
        mText->Transform()->SetRelativeRot(rot);
        mText->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));
        mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\icon_soul_white.png", L"texture\\HUD\\icon_soul_white.png", 0));

    }
    {
        CGameObject* mText = SpawnandReturnPrefab(L"prefab\\MoneyX.prefab", (int)LAYER::UI, Vec3(1500.f, -1150.f, 0.f)/3, 3.f);
        mText->SetName(L"MoneyX");
        mText->MeshRender()->GetDynamicMaterial(0);
        mText->Transform()->SetRelativeRot(rot);
        mText->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 0.f));
        mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_x.png", L"texture\\HUD\\n_x.png", 0));
     
    }
    Vec3 textpos;
    textpos = Vec3(1611.f, -1000.f, 0.f) / 3.f;
    //추가 되는 금액 출력 =================
    while (Money != 0)
    {
        DigitCount--;
        int digit = pow(10, DigitCount);
        int num = Money / digit;// 100/3
        Vec3 scale = Vec3(20.f, 20.f, 0.f);
        textpos += Vec3(5.f, 0.f, 0.f);
        textpos.z = 0.f;
        if (num == 1)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num1.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"1");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_1.png", L"texture\\HUD\\n_1.png", 0));

        }
        else if (num == 2)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num2.prefab", (int)LAYER::UI,textpos, 3.f);
            mText->SetName(L"2");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_2.png", L"texture\\HUD\\n_2.png", 0));

        }
        else if (num == 3)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num3.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"3");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_3.png", L"texture\\HUD\\n_3.png", 0));

        }
        else if (num == 4)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num4.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"4");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_4.png", L"texture\\HUD\\n_4.png", 0));

        }
        else if (num == 5)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num5.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"5");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_5.png", L"texture\\HUD\\n_5.png", 0));

        }
        else if (num == 6)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num6.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"6");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_6.png", L"texture\\HUD\\n_6.png", 0));
        }
        else if (num == 7)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num7.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"7");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_7.png", L"texture\\HUD\\n_7.png", 0));
        }
        else if (num == 8)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num8.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"8");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_8.png", L"texture\\HUD\\n_8.png", 0));
        }
        else if (num == 9)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num9.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"9");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_9.png", L"texture\\HUD\\n_9.png", 0));
        }
        else if (num == 0)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num0.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"0");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_0.png", L"texture\\HUD\\n_0.png", 0));
        }

        Money = Money % digit;// 100%3 알
        if (digit > 0 && 0 == Money)
        {
            int num = GetDigitCount(digit);
            num -= 1;
            for (size_t i = 0; i < num; i++)
            {
                textpos += Vec3(50.f, 0.f, 0.f);
                CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num0.prefab", (int)LAYER::UI, textpos, 3.f);
                mText->SetName(L"0");
                mText->MeshRender()->GetDynamicMaterial(0);
                mText->Transform()->SetRelativeRot(rot);
                mText->Transform()->SetRelativeScale(scale);
                mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_0.png", L"texture\\HUD\\n_0.png", 0));
            }
        }
    }


    CPlayerScript* pScript = CLevelMgr::GetInst()->FindObjectByName(L"Player")->GetScript<CPlayerScript>();
    int MoneyTotal = pScript->GetMoneyCount();
    //int MoneyTotal = 5260;
    int MTotalDigitCount = GetDigitCount(MoneyTotal);
    textpos = Vec3(1504.f, -1150.f, 0.f)/3;
    //player 현재 가지고 있는 금액 출력===============
    while (MoneyTotal != 0)
    {
        MTotalDigitCount--;
        int digit = pow(10, MTotalDigitCount);
        int num = MoneyTotal / digit;// 100/3
        Vec3 scale = Vec3(50.f, 50.f, 0.f);
        textpos += Vec3(40.f, 0.f, 0.f);
        textpos.z = 0.f;
        if (num == 1)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num1.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W1");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_1.png", L"texture\\HUD\\n_1.png", 0));
        }
        else if (num == 2)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num2.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W2");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_2.png", L"texture\\HUD\\n_2.png", 0));
        }
        else if (num == 3)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num3.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W3");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_3.png", L"texture\\HUD\\n_3.png", 0));
        }
        else if (num == 4)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num4.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W4");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_4.png", L"texture\\HUD\\n_4.png", 0));
        }
        else if (num == 5)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num5.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W5");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_5.png", L"texture\\HUD\\n_5.png", 0));
        }
        else if (num == 6)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num6.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W6");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_6.png", L"texture\\HUD\\n_6.png", 0));
        }
        else if (num == 7)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num7.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W7");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_7.png", L"texture\\HUD\\n_7.png", 0));
        }
        else if (num == 8)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num8.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W8");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_8.png", L"texture\\HUD\\n_8.png", 0));
        }
        else if (num == 9)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num9.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W9");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_9.png", L"texture\\HUD\\n_9.png", 0));
        }
        else if (num == 0)
        {
            CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num0.prefab", (int)LAYER::UI, textpos, 3.f);
            mText->SetName(L"W0");
            mText->MeshRender()->GetDynamicMaterial(0);
            mText->Transform()->SetRelativeRot(rot);
            mText->Transform()->SetRelativeScale(scale);
            mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_0.png", L"texture\\HUD\\n_0.png", 0));
        }

        MoneyTotal = MoneyTotal % digit;// 100%3 알
        if (digit > 0 && 0 == MoneyTotal)
        {
            int num = GetDigitCount(digit);
            num -= 1;
            for (size_t i = 0; i < num; i++)
            {
                textpos += Vec3(40.f, 0.f, 0.f);
                CGameObject* mText = SpawnandReturnPrefab(L"prefab\\num0.prefab", (int)LAYER::UI, textpos, 3.f);
                mText->SetName(L"W0");
                mText->MeshRender()->GetDynamicMaterial(0);
                mText->Transform()->SetRelativeRot(rot);
                mText->Transform()->SetRelativeScale(scale);
                mText->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->LoadTexture(L"texture\\HUD\\n_0.png", L"texture\\HUD\\n_0.png", 0));
            }
        }
    }
}

void CLevelSaveLoadInScript::AddChild(CGameObject* _pOwner, CGameObject* _pChild)
{
    tEvent evn = {};
    evn.Type = EVENT_TYPE::ADD_CHILD;
    evn.wParam = (DWORD_PTR)_pOwner;
    evn.lParam = (DWORD_PTR)_pChild;
    CEventMgr::GetInst()->AddEvent(evn);
}

