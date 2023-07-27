#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE			{ MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID		{ ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO				{ INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION			{ ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID			{ TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID			{ RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_EFFECT, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE		{ DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE		{ DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum class CAMERA_TYPE	{ PERSPECTIVE, ORTHOGRAPHIC, CAMERA_END};
	enum class LIGHT_TYPE	{ LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_SPOT, LIGHT_END };

	enum class EVENT_TYPE	{ CREATE_OBJECT, DELETE_OBJECT, SCENE_CHANGE, EVENT_END };

	enum class SCENETAG		{ LOGO, LOADING, VILLAGE, STAGE, BOSSSTAGE, EDITOR, SCENE_END};
	enum class LAYERTAG		{ EVERYTHING, ENVIRONMENT, GAMELOGIC, EFFECT, UI, IGNORERAY, LAYER_END };
	//enum class LAYERTAG	{ EVERYTHING, ENVIRONMENT, DYNAMIC_GAMEOBJECT, STATIC_GAMEOBJECT, EFFECT, UI, IGNORERAY, LAYER_END };

	enum class OBJECTTAG	{ BACKGROUND, PLAYER, BLOCK, ITEM, IMMORTAL, FRAGILE, MONSTER, BOSS, CAMERA, UI, SKYBOX, PLAYERBULLET, MONSTERBULLET, SPAWNINGPOOL, NPC, EFFECT, TRAP, RAY, TRIGERBLOCK, LOSTSOUL, OBJECT_END };

	enum class ITEMTAG		{ WEAPON, ARMOR, WANDS, POTION, SHIELD, ACCESSORY, CONSUMABLE, GEM, ARTIFACT, ITEM_END };
	enum class COMPONENTTAG	{ BUFFER, TEXTURE0, TEXTURE1, TRANSFORM, COLLIDER, RIGIDBODY, LIGHT, CAMERA, BASICSTAT, MONSTERAI,
								ANIMATOR, PARTICLE, BILLBOARD, STATEMACHINE, INVENTORY, FONT, COMPONENT_END };
	
	// STATE
	enum class STATE		{ IDLE, ROMIMG, ATTACK, HIT, DEAD, SEMI_PATTERN1, SEMI_PATTERN2,
								BOSS_SLEEP, BOSS_WAKEUP, BOSS_IDLE, BOSS_STURN, BOSS_ATTACK, BOSS_ATTACK1, BOSS_ATTACK2, BOSS_ATTACK3,
								BOSS_PH1SKILL1, BOSS_PH1SKILL2, BOSS_PH1SKILL3, BOSS_PH1SKILL4, BOSS_PH1SKILL5,
								BOSS_PH2SKILL1, BOSS_PH2SKILL2, BOSS_PH2SKILL3, BOSS_PH2SKILL4, BOSS_PH2SKILL5,
								BOSS_PH3SKILL1, BOSS_PH3SKILL2, BOSS_PH3SKILL3, BOSS_PH3SKILL4, BOSS_PH3SKILL5,
								BOSS_METEORREADY,BOSS_CRAWL, BOSS_SPWANMONSTER, BOSS_DYING, BOSS_DEAD, BOSS_TELEPORT, STATE_END };
	enum class MONSTERTAG	{ SPIDER, WARRIOR, BAT, WIZARD, ALIEN, SLIME, SKELETON, SKULLGHOST, WORM, MONSTER_END };
	enum class TRAPTAG		{ BLADE, STRIKEDOWN, JUMP, TRAP_END };
	enum class ENVIRONMENTTAG { TREE, ROCK, GRASS, MUSHROOM, PUMPKIN, ETC, ENVIRONMENT_END };
	enum class FRAGILETAG	{ RANDOMBOX, EQUIPBOX, FRAGILE_END};

	enum UILAYER            { UI_DOWN, UI_MIDDLE, UI_UP, UI_END };
	enum UIPOPUPLAYER       { POPUP_MOUSE, POPUP_INVEN, POPUP_EQUIPMENT, POPUP_STAT, POPUP_ITEM, POPUP_MAP, POPUP_ESC, POPUP_SPEECH, POPUP_SHOP, POPUP_END };
	enum UIOBJECTTTAG       { UIID_BASIC, UIID_SLOTBASIC, UIID_SLOTEMPTY, UIID_SLOTEQUIPMENT, UIID_INVENBUTTON, UIID_END };

	enum INVENKEYSLOT       { KEYSLOT_ONE, KEYSLOT_TWO, KEYSLOT_THREE, KEYSLOT_FOUR, KEYSLOT_FIVE, KEYSLOT_END };
	enum INVENITEMSLOT      { ITEMSLOT_HELMET, ITEMSLOT_HAND, ITEMSLOT_ARMOR, ITEMSLOT_RING, ITEMSLOT_PANTS, ITEMSLOT_NECKLACE, ITEMSLOT_END };
	
	// 아이템 아이디 일단 다 추가해 둘 테니 중요한 순서대로 만들어 주십쇼 :D
	enum ITEMTYPE           { ITEMTYPE_WEAPONITEM, ITEMTYPE_GENERALITEM, ITEMTYPE_EQUIPITEM, ITEMTYPE_EATITEM, ITEMTYPE_POTIONITEM, ITEMTYPE_END};
	
	enum ITEMID             { WEAPON_SWORD,                                    // 근접 무기 칼
		WEAPON_BOW, WEAPON_ARROW, WEAPON_BOWANDARROW,                          // 원거리 무기 활, 화살, 활과 화살
		WEAPON_WAND1, WEAPON_WAND2, WEAPON_WAND3, WEAPON_WAND4, WEAPON_WAND5,  // 지팡이 무기 5가지

		GENERAL_LAMP, GENERAL_BEER, GENERAL_KEY, GENERAL_DOCUMENT, GENERAL_MAP,// 일반 아이템 램프, 맥주, 열쇠, 문서, 지도
		GENERAL_SHIELD,

		EQUIP_OLDHELMET, EQUIP_OLDARMOR, EQUIP_OLDTROUSERS,                    // 낡은 투구, 낡은 갑옷, 낡은 바지
		EQUIP_IRONHELMET, EQUIP_IRONARMOR, EQUIP_IRONTROUSERS,                 // 철 투구, 철 갑옷, 철 바지
		EQUIP_SMALLSILVERRING, EQUIP_BIGSILVERRING, EQUIP_SILVERNECKLACE,      // 작은 실버 반지, 큰 실버 반지, 실버 목걸이
		EQUIP_SMALLGOLDRING, EQUIP_BIGGOLDRING, EQUIP_GOLDNECKLACE,            // 작은 골드 반지, 큰 골드 반지, 골드 목걸이

		EAT_SLICEDBREAD, EAT_APPLE, EAT_CHEESE, EAT_RAWMEAT, EAT_COOKEDMEAT,   // 식빵, 사과, 치즈, 안 익은 고기, 익은 고기

		EAT_POTION1, EAT_POTION2, EAT_POTION3, EAT_POTION4, EAT_POTION5, EAT_POTION6, EAT_POTION7,        // 포션1, 포션2, 포션3, 포션4, 포션5 : 능력 및 효과는 미정

		WEAPON_EPICBOW,

		ITEMID_END
	};
	enum class NPCTAG		{ TRADER, ALCHEMIST, BARD, OLD_MAN, DOG, WIZARD, NPC_END}; //상인, 연금술사(포션), 잡NPC, 음유시인, 노인, 개, 마법사(흑막예정)

	enum EFFECTCOLOR        { ECOLOR_NONE, ECOLOR_PINK ,ECOLOR_RED, ECOLOR_ORANGE, ECOLOR_YELLOW, ECOLOR_LIGHTGREEN, ECOLOR_GREEN, ECOLOR_SKYBLUE, ECOLOR_BLUE, ECOLOR_INDIGO, ECOLOR_PURPLE, ECOLOR_WHITE, ECOLOR_RAINBOW, ECOLOR_BROWN, ECOLOR_APRICOT, ECOLOR_END };
	enum EFFECTTAG          { EFFECT_BUBBLE, EFFECT_BROKENBOX, EFFECT_DAMAGE, EFFECT_DAMAGESTAR, EFFECT_BLOOD, EFFECT_WAND, EFFECT_EXPLOSION, EFFECT_EXPLOSIONCHILD, EFFECT_PROJECTILETRACE, EFFECT_TRACE, EFFECT_TWINKLE, EFFECT_PASTTRACE, EFFECT_STAR , EFFECT_BONEFIRE, EFFECT_LEAF, EFFECT_LEAVES, EFFECT_FIREFLY, EFFECT_END };
	enum PARTICLETAG        { PARTICLE_SQUARE, PARTICLE_END };

	enum class BOSSPHASE	{PHASE1, PHASE2, PHASE3, LASTPHASE, PHASE_END};

	enum class SOULSTATE { SOUL_NORMAL, SOUL_PARRY, SOUL_END };//기본, 패링당했을 때

	enum CHANNELID          
	{ SOUND_EFFECT, SOUND_BGM, SOUND_WEAPON, SOUND_UI, SOUND_PLAYER, 
		SOUND_CONSUMABLE, SOUND_INVENTORY, SOUND_ENVIRONMENT, SOUND_BREAK, 
		SOUND_WARRIOR, SOUND_SPIDER, SOUND_BAT, SOUND_WIZARD, SOUND_ALIEN, 
		SOUND_SLIME, SOUND_SKELETON, SOUND_SKULL, SOUND_WORM,
		
		SOUND_MAXCHANNEL };
};
#endif // Engine_Enum_h__
