#include "pch.h"
#include "CKnightSpinAttackCombo.h"

CKnightSpinAttackCombo::CKnightSpinAttackCombo()
{
}

CKnightSpinAttackCombo::~CKnightSpinAttackCombo()
{
}

void CKnightSpinAttackCombo::Enter()
{
	GetOwner()->Animator3D()->Play(13, false);
}

void CKnightSpinAttackCombo::tick()
{
}

void CKnightSpinAttackCombo::Exit()
{
}
