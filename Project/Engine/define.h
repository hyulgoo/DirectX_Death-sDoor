#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define CAMERASHAKE(range, speed, term) CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript<CGameCameraScript>()->CameraShake(range, speed, term)

#define MAX_LAYER 32
#define MAX_MIP 8

#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>;



enum class MRT_TYPE
{
	SWAPCHAIN,
	DEFERRED,	
	DECAL,
	LIGHT,
	SHADOW,
	WATER,
	END,
};



enum class COMPONENT_TYPE
{
	// update
	TRANSFORM,		// 위치, 크기, 회전
	COLLIDER2D,		// 2차원 충돌
	COLLIDER3D,		// 3차원 충돌
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	CAMERA,			// Camera
	RIGIDBODY,

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM, // 입자 렌더링
	TILEMAP,		// 2차원 타일
	SKYBOX,			// 하늘
	LANDSCAPE,		// 3차원 지형
	DECAL,			// 내부 렌더링
		
	END,

	// custom
	SCRIPT,
};

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END];


enum class RES_TYPE
{
	MESHDATA,
	MATERIAL,
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END];




enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
	END,
};


enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,	
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};

enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,	

	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,	
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X

	FRONT_CHECK,

	END,
};


enum class BS_TYPE
{
	DEFAULT,		// No Blending
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수 
	ONE_ONE,		// 1:1 혼합
	END,
};





enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,	
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class SHADER_DOMAIN
{
	// Deferred
	DOMAIN_DEFERRED,
	DOMAIN_DECAL,

	// Light
	DOMAIN_LIGHT,

	// SwapChain(Foward)
	DOMAIN_OPAQUE,		// 불투명 오브젝트
	DOMAIN_MASK,		// 불투명, 투명
	DOMAIN_TRANSPARENT,	// 반투명
	DOMAIN_POSTPROCESS, // 후 처리
	DOMAIN_UI,
	DOMAIN_UNDEFINED,	// 미정
};


enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Index
	DELETE_OBJECT,  // wParam : GameObject

	ADD_CHILD,

	DELETE_RESOURCE,	// wParam : RES_TYPE, lParam : Resource Adress

	LEVEL_CHANGE,	// wParam : Level, LParam : LEVEL_TYPE
};


enum class SHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	FRUSTUM,
	CAPSULE,
	END,
};


enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};

enum class COLLIDER3D_TYPE
{
	CUBE,
	SPHERE,
};


enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class SB_TYPE
{
	READ_ONLY,
	READ_WRITE,
};

enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN,
	COLOR_CHANGE,
	SCALE_CHANGE,
	ADD_VELOCITY,

	DRAG,
	NOISE_FORCE,
	RENDER,
	ANIMATION,

	END,
};

enum class LAYER
{
	DEFAULT,
	MAINCAMERA,
	SUBCAMERA,
	PLAYER,
	MONSTER,
	PLAYERPROJECTILE,
	MONSTERPROJECTILE,
	WALL,
	ITEM,
	GROUND,
	FALLAREA,
	LADDER, 
	ANCHOR,
	BRAIZER,
	NPC, 
	LEVELCHANGEDOOR,

	UI = 31,
};

enum class LANDSCAPE_MOD
{
	HEIGHT_MAP,
	SPLAT,
	NONE,
};

enum class LEVEL_TYPE
{
	CASTLE_FIELD,
	CASTLE_BOSS,
	FOREST_FIELD,
	ICE_FIELD,
	ICE_BOSS,
	HALL,
	LOADING,
	START,
	END
};

enum class PLAYERANIM_TYPE
{
	IDLE,
	LADDER_DOWN,
	LADDER_UP,
	LADDER_FINISH,
	FALL,
	HOOK,
	HOOKING,
	MAGIC_FIRE,
	MAGIC_BOMB,
	MAGIC_BOMB_FINISH,
	GETITEM,
	DANCE,
	PICKITEM,
	SLASH_R,
	SLASH_L,
	WALK,
	RUN,
	CHARGE_L,
	CHARGE_R,
	CHARGE_MAX_R,
	DODGE,
	DODGE_SLASH,
	HIT,
	HIT_RECOVER,
	HIT_IDLE,
	CHARGE_MAX_L,
	CHARGE_ATTACK_R,
	CHARGE_ATTACK_L,
	ARROW,
	END,
};

extern LEVEL_TYPE g_tNextLevel;

enum class PLAYER_MAGIC
{
	ARROW,
	FIRE,
	BOMB,
	HOOK,
	END,
};

enum class PLAYER_UPGRADE
{
	ATTACK,
	ATK_SPEED,
	SPEED,
	MAGIC,
	END,
};

enum class CHAINPATERN
{
	ONE,
	CROSS,
	SPREAD,
};
