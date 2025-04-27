#include "pch.h"
#include "CKnightDeath.h"
#include "CKnightScript.h"
#include "CPlayerScript.h"
#include "CLevelSaveLoadInScript.h"
#include "CSoundScript.h"
#include "CLevelChangeDoorScript.h"

CKnightDeath::CKnightDeath() :
    m_bStartPaperBurn(false),
    m_fPaperBurnTime(0.f)
{
}

CKnightDeath::~CKnightDeath()
{
}

void CKnightDeath::Enter()
{
    GetOwner()->Animator3D()->Play(3, false);
    CSoundScript* pSoundscript = CLevelMgr::GetInst()->FindObjectByName(L"SoundUI")->GetScript<CSoundScript>();
    Ptr<CSound> pSound = pSoundscript->AddSound(L"Sound\\Monster\\Knight\\KnightDeath.ogg", 1, 0.2f);
}

void CKnightDeath::tick()
{
    // 애니메이션이 끝난 경우 사망 Paperburn 효과 주기.
    if (GetOwner()->Animator3D()->IsFinish())
    {
        GetOwner()->GetScript<CKnightScript>()->SetPaperBurnEffect(true);
        m_bStartPaperBurn = true;
    }

    if (m_bStartPaperBurn)
        m_fPaperBurnTime += DT;

    if (m_fPaperBurnTime > 3.f && !GetOwner()->IsDead())
    {
        GetOwner()->GetScript<CKnightScript>()->GetPlayer()->GetScript<CPlayerScript>()->AddMoney((UINT)600);
        CLevelSaveLoadInScript script;
        script.MoneyCount(600);
        CGameObject* pDoor = CLevelSaveLoadInScript::SpawnandReturnPrefab(L"prefab\\LevelChangeDoor.prefab",(int)LAYER::LEVELCHANGEDOOR,Vec3(3200.f, 1200.f, 2880.f));
        pDoor->Transform()->SetRelativePos(Vec3(3200.f, 1050.f, 2880.f));
        pDoor->Transform()->SetRelativeRot({ 0.f, 45.f, 0.f });
        pDoor->GetScript<CLevelChangeDoorScript>()->SetLevelType((int)LEVEL_TYPE::HALL);
        GetOwnerScript()->Destroy();
    }
    GetOwner()->Rigidbody()->ClearForce();
}

void CKnightDeath::Exit()
{
}
