#include "pch.h"
#include "CPlyIdle.h"
#include "CPlayerScript.h"

CPlyIdle::CPlyIdle()
{
}

CPlyIdle::~CPlyIdle()
{
}

void CPlyIdle::tick()
{

	GetOwner()->Rigidbody()->ClearForce();

	// 상점 이용 중이라 이동이 막혀있다면 상태를 전환하지 않음.
	if (!GetOwner()->GetScript<CPlayerScript>()->IsAbleMove())
		return;

	if (KEY_TAP(KEY::LBTN))
	{
		GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Attack");
	}
	else if (KEY_TAP(KEY::RBTN))
	{
		GetOwner()->GetScript<CPlayerScript>()->ChangeMagicState();
	}
	// 이동키를 눌렀다면 Walk 상태로 전환
	else if (KEY_PRESSED(KEY::W) || KEY_PRESSED(KEY::S) || KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::D))
	{
		GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Run");
	}	
	else if (KEY_TAP(KEY::SPACE))
	{
		GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Dodge");
	}
}

void CPlyIdle::Enter()
{
	// Idle 진입 시 Idle Anim 반복재생
	GetOwner()->Animator3D()->Play((int)PLAYERANIM_TYPE::IDLE, true);
}

void CPlyIdle::Exit()
{
}

void CPlyIdle::BeginOverlap(CCollider3D* _Other)
{
}

void CPlyIdle::OnOverlap(CCollider3D* _Other)
{
	if (_Other->GetOwner()->GetLayerIndex() == (int)LAYER::LADDER && KEY_PRESSED(KEY::E))
		GetOwner()->GetScript<CPlayerScript>()->ChangeState(L"Ladder");
}
