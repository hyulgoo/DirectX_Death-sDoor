#include "pch.h"
#include "CScriptMgr.h"

#include "CAnchorScript.h"
#include "CArrowIconScript.h"
#include "CBankerFrameScript.h"
#include "CBankNPCScript.h"
#include "CBankUpgradeScript.h"
#include "CBatScript.h"
#include "CBazookaGasGrenadeScript.h"
#include "CBazookaScript.h"
#include "CBombIconScript.h"
#include "CBossChainScript.h"
#include "CBrazierScript.h"
#include "CCameraMoveScript.h"
#include "CCrowBossScript.h"
#include "CCrowHeadScript.h"
#include "CCursorScript.h"
#include "CEnterScript.h"
#include "CFenceScript.h"
#include "CFireIconScript.h"
#include "CGameCameraScript.h"
#include "CGhostScript.h"
#include "CGrimKnightScript.h"
#include "CGruntScript.h"
#include "CHitStoneScript.h"
#include "CHookIconScript.h"
#include "CHPIconScript.h"
#include "CHUDScript.h"
#include "CKnightScript.h"
#include "CLadderScript.h"
#include "CLevelChangeDoorScript.h"
#include "CLoadLevelThreadScript.h"
#include "CLurkerScript.h"
#include "CMagic_ArrowScript.h"
#include "CMagic_BombScript.h"
#include "CMagic_FireScript.h"
#include "CMagic_HookScript.h"
#include "CMainLightScript.h"
#include "CMoneyCountScript.h"
#include "CMonsterDetectRangeScript.h"
#include "CMPIconScript.h"
#include "CPlayerScript.h"
#include "CPlayerWeaponScript.h"
#include "CRoomScript.h"
#include "CSlashScript.h"
#include "CSoundScript.h"
#include "CSpawnDoorScript.h"
#include "CStartSceneUIScript.h"
#include "CStateScript.h"
#include "CTrapScript.h"
#include "CWaterCameraScript.h"
#include "CWaterScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAnchorScript");
	_vec.push_back(L"CArrowIconScript");
	_vec.push_back(L"CBankerFrameScript");
	_vec.push_back(L"CBankNPCScript");
	_vec.push_back(L"CBankUpgradeScript");
	_vec.push_back(L"CBatScript");
	_vec.push_back(L"CBazookaGasGrenadeScript");
	_vec.push_back(L"CBazookaScript");
	_vec.push_back(L"CBombIconScript");
	_vec.push_back(L"CBossChainScript");
	_vec.push_back(L"CBrazierScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCrowBossScript");
	_vec.push_back(L"CCrowHeadScript");
	_vec.push_back(L"CCursorScript");
	_vec.push_back(L"CEnterScript");
	_vec.push_back(L"CFenceScript");
	_vec.push_back(L"CFireIconScript");
	_vec.push_back(L"CGameCameraScript");
	_vec.push_back(L"CGhostScript");
	_vec.push_back(L"CGrimKnightScript");
	_vec.push_back(L"CGruntScript");
	_vec.push_back(L"CHitStoneScript");
	_vec.push_back(L"CHookIconScript");
	_vec.push_back(L"CHPIconScript");
	_vec.push_back(L"CHUDScript");
	_vec.push_back(L"CKnightScript");
	_vec.push_back(L"CLadderScript");
	_vec.push_back(L"CLevelChangeDoorScript");
	_vec.push_back(L"CLoadLevelThreadScript");
	_vec.push_back(L"CLurkerScript");
	_vec.push_back(L"CMagic_ArrowScript");
	_vec.push_back(L"CMagic_BombScript");
	_vec.push_back(L"CMagic_FireScript");
	_vec.push_back(L"CMagic_HookScript");
	_vec.push_back(L"CMainLightScript");
	_vec.push_back(L"CMoneyCountScript");
	_vec.push_back(L"CMonsterDetectRangeScript");
	_vec.push_back(L"CMPIconScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerWeaponScript");
	_vec.push_back(L"CRoomScript");
	_vec.push_back(L"CSlashScript");
	_vec.push_back(L"CSoundScript");
	_vec.push_back(L"CSpawnDoorScript");
	_vec.push_back(L"CStartSceneUIScript");
	_vec.push_back(L"CStateScript");
	_vec.push_back(L"CTrapScript");
	_vec.push_back(L"CWaterCameraScript");
	_vec.push_back(L"CWaterScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAnchorScript" == _strScriptName)
		return new CAnchorScript;
	if (L"CArrowIconScript" == _strScriptName)
		return new CArrowIconScript;
	if (L"CBankerFrameScript" == _strScriptName)
		return new CBankerFrameScript;
	if (L"CBankNPCScript" == _strScriptName)
		return new CBankNPCScript;
	if (L"CBankUpgradeScript" == _strScriptName)
		return new CBankUpgradeScript;
	if (L"CBatScript" == _strScriptName)
		return new CBatScript;
	if (L"CBazookaGasGrenadeScript" == _strScriptName)
		return new CBazookaGasGrenadeScript;
	if (L"CBazookaScript" == _strScriptName)
		return new CBazookaScript;
	if (L"CBombIconScript" == _strScriptName)
		return new CBombIconScript;
	if (L"CBossChainScript" == _strScriptName)
		return new CBossChainScript;
	if (L"CBrazierScript" == _strScriptName)
		return new CBrazierScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCrowBossScript" == _strScriptName)
		return new CCrowBossScript;
	if (L"CCrowHeadScript" == _strScriptName)
		return new CCrowHeadScript;
	if (L"CCursorScript" == _strScriptName)
		return new CCursorScript;
	if (L"CEnterScript" == _strScriptName)
		return new CEnterScript;
	if (L"CFenceScript" == _strScriptName)
		return new CFenceScript;
	if (L"CFireIconScript" == _strScriptName)
		return new CFireIconScript;
	if (L"CGameCameraScript" == _strScriptName)
		return new CGameCameraScript;
	if (L"CGhostScript" == _strScriptName)
		return new CGhostScript;
	if (L"CGrimKnightScript" == _strScriptName)
		return new CGrimKnightScript;
	if (L"CGruntScript" == _strScriptName)
		return new CGruntScript;
	if (L"CHitStoneScript" == _strScriptName)
		return new CHitStoneScript;
	if (L"CHookIconScript" == _strScriptName)
		return new CHookIconScript;
	if (L"CHPIconScript" == _strScriptName)
		return new CHPIconScript;
	if (L"CHUDScript" == _strScriptName)
		return new CHUDScript;
	if (L"CKnightScript" == _strScriptName)
		return new CKnightScript;
	if (L"CLadderScript" == _strScriptName)
		return new CLadderScript;
	if (L"CLevelChangeDoorScript" == _strScriptName)
		return new CLevelChangeDoorScript;
	if (L"CLoadLevelThreadScript" == _strScriptName)
		return new CLoadLevelThreadScript;
	if (L"CLurkerScript" == _strScriptName)
		return new CLurkerScript;
	if (L"CMagic_ArrowScript" == _strScriptName)
		return new CMagic_ArrowScript;
	if (L"CMagic_BombScript" == _strScriptName)
		return new CMagic_BombScript;
	if (L"CMagic_FireScript" == _strScriptName)
		return new CMagic_FireScript;
	if (L"CMagic_HookScript" == _strScriptName)
		return new CMagic_HookScript;
	if (L"CMainLightScript" == _strScriptName)
		return new CMainLightScript;
	if (L"CMoneyCountScript" == _strScriptName)
		return new CMoneyCountScript;
	if (L"CMonsterDetectRangeScript" == _strScriptName)
		return new CMonsterDetectRangeScript;
	if (L"CMPIconScript" == _strScriptName)
		return new CMPIconScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerWeaponScript" == _strScriptName)
		return new CPlayerWeaponScript;
	if (L"CRoomScript" == _strScriptName)
		return new CRoomScript;
	if (L"CSlashScript" == _strScriptName)
		return new CSlashScript;
	if (L"CSoundScript" == _strScriptName)
		return new CSoundScript;
	if (L"CSpawnDoorScript" == _strScriptName)
		return new CSpawnDoorScript;
	if (L"CStartSceneUIScript" == _strScriptName)
		return new CStartSceneUIScript;
	if (L"CStateScript" == _strScriptName)
		return new CStateScript;
	if (L"CTrapScript" == _strScriptName)
		return new CTrapScript;
	if (L"CWaterCameraScript" == _strScriptName)
		return new CWaterCameraScript;
	if (L"CWaterScript" == _strScriptName)
		return new CWaterScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ANCHORSCRIPT:
		return new CAnchorScript;
		break;
	case (UINT)SCRIPT_TYPE::ARROWICONSCRIPT:
		return new CArrowIconScript;
		break;
	case (UINT)SCRIPT_TYPE::BANKERFRAMESCRIPT:
		return new CBankerFrameScript;
		break;
	case (UINT)SCRIPT_TYPE::BANKNPCSCRIPT:
		return new CBankNPCScript;
		break;
	case (UINT)SCRIPT_TYPE::BANKUPGRADESCRIPT:
		return new CBankUpgradeScript;
		break;
	case (UINT)SCRIPT_TYPE::BATSCRIPT:
		return new CBatScript;
		break;
	case (UINT)SCRIPT_TYPE::BAZOOKAGASGRENADESCRIPT:
		return new CBazookaGasGrenadeScript;
		break;
	case (UINT)SCRIPT_TYPE::BAZOOKASCRIPT:
		return new CBazookaScript;
		break;
	case (UINT)SCRIPT_TYPE::BOMBICONSCRIPT:
		return new CBombIconScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSCHAINSCRIPT:
		return new CBossChainScript;
		break;
	case (UINT)SCRIPT_TYPE::BRAZIERSCRIPT:
		return new CBrazierScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CROWBOSSSCRIPT:
		return new CCrowBossScript;
		break;
	case (UINT)SCRIPT_TYPE::CROWHEADSCRIPT:
		return new CCrowHeadScript;
		break;
	case (UINT)SCRIPT_TYPE::CURSORSCRIPT:
		return new CCursorScript;
		break;
	case (UINT)SCRIPT_TYPE::ENTERSCRIPT:
		return new CEnterScript;
		break;
	case (UINT)SCRIPT_TYPE::FENCESCRIPT:
		return new CFenceScript;
		break;
	case (UINT)SCRIPT_TYPE::FIREICONSCRIPT:
		return new CFireIconScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMECAMERASCRIPT:
		return new CGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::GHOSTSCRIPT:
		return new CGhostScript;
		break;
	case (UINT)SCRIPT_TYPE::GRIMKNIGHTSCRIPT:
		return new CGrimKnightScript;
		break;
	case (UINT)SCRIPT_TYPE::GRUNTSCRIPT:
		return new CGruntScript;
		break;
	case (UINT)SCRIPT_TYPE::HITSTONESCRIPT:
		return new CHitStoneScript;
		break;
	case (UINT)SCRIPT_TYPE::HOOKICONSCRIPT:
		return new CHookIconScript;
		break;
	case (UINT)SCRIPT_TYPE::HPICONSCRIPT:
		return new CHPIconScript;
		break;
	case (UINT)SCRIPT_TYPE::HUDSCRIPT:
		return new CHUDScript;
		break;
	case (UINT)SCRIPT_TYPE::KNIGHTSCRIPT:
		return new CKnightScript;
		break;
	case (UINT)SCRIPT_TYPE::LADDERSCRIPT:
		return new CLadderScript;
		break;
	case (UINT)SCRIPT_TYPE::LEVELCHANGEDOORSCRIPT:
		return new CLevelChangeDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::LOADLEVELTHREADSCRIPT:
		return new CLoadLevelThreadScript;
		break;
	case (UINT)SCRIPT_TYPE::LURKERSCRIPT:
		return new CLurkerScript;
		break;
	case (UINT)SCRIPT_TYPE::MAGIC_ARROWSCRIPT:
		return new CMagic_ArrowScript;
		break;
	case (UINT)SCRIPT_TYPE::MAGIC_BOMBSCRIPT:
		return new CMagic_BombScript;
		break;
	case (UINT)SCRIPT_TYPE::MAGIC_FIRESCRIPT:
		return new CMagic_FireScript;
		break;
	case (UINT)SCRIPT_TYPE::MAGIC_HOOKSCRIPT:
		return new CMagic_HookScript;
		break;
	case (UINT)SCRIPT_TYPE::MAINLIGHTSCRIPT:
		return new CMainLightScript;
		break;
	case (UINT)SCRIPT_TYPE::MONEYCOUNTSCRIPT:
		return new CMoneyCountScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERDETECTRANGESCRIPT:
		return new CMonsterDetectRangeScript;
		break;
	case (UINT)SCRIPT_TYPE::MPICONSCRIPT:
		return new CMPIconScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERWEAPONSCRIPT:
		return new CPlayerWeaponScript;
		break;
	case (UINT)SCRIPT_TYPE::ROOMSCRIPT:
		return new CRoomScript;
		break;
	case (UINT)SCRIPT_TYPE::SLASHSCRIPT:
		return new CSlashScript;
		break;
	case (UINT)SCRIPT_TYPE::SOUNDSCRIPT:
		return new CSoundScript;
		break;
	case (UINT)SCRIPT_TYPE::SPAWNDOORSCRIPT:
		return new CSpawnDoorScript;
		break;
	case (UINT)SCRIPT_TYPE::STARTSCENEUISCRIPT:
		return new CStartSceneUIScript;
		break;
	case (UINT)SCRIPT_TYPE::STATESCRIPT:
		return new CStateScript;
		break;
	case (UINT)SCRIPT_TYPE::TRAPSCRIPT:
		return new CTrapScript;
		break;
	case (UINT)SCRIPT_TYPE::WATERCAMERASCRIPT:
		return new CWaterCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::WATERSCRIPT:
		return new CWaterScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ANCHORSCRIPT:
		return L"CAnchorScript";
		break;

	case SCRIPT_TYPE::ARROWICONSCRIPT:
		return L"CArrowIconScript";
		break;

	case SCRIPT_TYPE::BANKERFRAMESCRIPT:
		return L"CBankerFrameScript";
		break;

	case SCRIPT_TYPE::BANKNPCSCRIPT:
		return L"CBankNPCScript";
		break;

	case SCRIPT_TYPE::BANKUPGRADESCRIPT:
		return L"CBankUpgradeScript";
		break;

	case SCRIPT_TYPE::BATSCRIPT:
		return L"CBatScript";
		break;

	case SCRIPT_TYPE::BAZOOKAGASGRENADESCRIPT:
		return L"CBazookaGasGrenadeScript";
		break;

	case SCRIPT_TYPE::BAZOOKASCRIPT:
		return L"CBazookaScript";
		break;

	case SCRIPT_TYPE::BOMBICONSCRIPT:
		return L"CBombIconScript";
		break;

	case SCRIPT_TYPE::BOSSCHAINSCRIPT:
		return L"CBossChainScript";
		break;

	case SCRIPT_TYPE::BRAZIERSCRIPT:
		return L"CBrazierScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CROWBOSSSCRIPT:
		return L"CCrowBossScript";
		break;

	case SCRIPT_TYPE::CROWHEADSCRIPT:
		return L"CCrowHeadScript";
		break;

	case SCRIPT_TYPE::CURSORSCRIPT:
		return L"CCursorScript";
		break;

	case SCRIPT_TYPE::ENTERSCRIPT:
		return L"CEnterScript";
		break;

	case SCRIPT_TYPE::FENCESCRIPT:
		return L"CFenceScript";
		break;

	case SCRIPT_TYPE::FIREICONSCRIPT:
		return L"CFireIconScript";
		break;

	case SCRIPT_TYPE::GAMECAMERASCRIPT:
		return L"CGameCameraScript";
		break;

	case SCRIPT_TYPE::GHOSTSCRIPT:
		return L"CGhostScript";
		break;

	case SCRIPT_TYPE::GRIMKNIGHTSCRIPT:
		return L"CGrimKnightScript";
		break;

	case SCRIPT_TYPE::GRUNTSCRIPT:
		return L"CGruntScript";
		break;

	case SCRIPT_TYPE::HITSTONESCRIPT:
		return L"CHitStoneScript";
		break;

	case SCRIPT_TYPE::HOOKICONSCRIPT:
		return L"CHookIconScript";
		break;

	case SCRIPT_TYPE::HPICONSCRIPT:
		return L"CHPIconScript";
		break;

	case SCRIPT_TYPE::HUDSCRIPT:
		return L"CHUDScript";
		break;

	case SCRIPT_TYPE::KNIGHTSCRIPT:
		return L"CKnightScript";
		break;

	case SCRIPT_TYPE::LADDERSCRIPT:
		return L"CLadderScript";
		break;

	case SCRIPT_TYPE::LEVELCHANGEDOORSCRIPT:
		return L"CLevelChangeDoorScript";
		break;

	case SCRIPT_TYPE::LOADLEVELTHREADSCRIPT:
		return L"CLoadLevelThreadScript";
		break;

	case SCRIPT_TYPE::LURKERSCRIPT:
		return L"CLurkerScript";
		break;

	case SCRIPT_TYPE::MAGIC_ARROWSCRIPT:
		return L"CMagic_ArrowScript";
		break;

	case SCRIPT_TYPE::MAGIC_BOMBSCRIPT:
		return L"CMagic_BombScript";
		break;

	case SCRIPT_TYPE::MAGIC_FIRESCRIPT:
		return L"CMagic_FireScript";
		break;

	case SCRIPT_TYPE::MAGIC_HOOKSCRIPT:
		return L"CMagic_HookScript";
		break;

	case SCRIPT_TYPE::MAINLIGHTSCRIPT:
		return L"CMainLightScript";
		break;

	case SCRIPT_TYPE::MONEYCOUNTSCRIPT:
		return L"CMoneyCountScript";
		break;

	case SCRIPT_TYPE::MONSTERDETECTRANGESCRIPT:
		return L"CMonsterDetectRangeScript";
		break;

	case SCRIPT_TYPE::MPICONSCRIPT:
		return L"CMPIconScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERWEAPONSCRIPT:
		return L"CPlayerWeaponScript";
		break;

	case SCRIPT_TYPE::ROOMSCRIPT:
		return L"CRoomScript";
		break;

	case SCRIPT_TYPE::SLASHSCRIPT:
		return L"CSlashScript";
		break;

	case SCRIPT_TYPE::SOUNDSCRIPT:
		return L"CSoundScript";
		break;

	case SCRIPT_TYPE::SPAWNDOORSCRIPT:
		return L"CSpawnDoorScript";
		break;

	case SCRIPT_TYPE::STARTSCENEUISCRIPT:
		return L"CStartSceneUIScript";
		break;

	case SCRIPT_TYPE::STATESCRIPT:
		return L"CStateScript";
		break;

	case SCRIPT_TYPE::TRAPSCRIPT:
		return L"CTrapScript";
		break;

	case SCRIPT_TYPE::WATERCAMERASCRIPT:
		return L"CWaterCameraScript";
		break;

	case SCRIPT_TYPE::WATERSCRIPT:
		return L"CWaterScript";
		break;

	}
	return nullptr;
}