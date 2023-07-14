#include "stdafx.h"
#include "Loading.h"
#include "Export_Function.h"
CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}


CLoading::~CLoading()
{

}

unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint	iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADINGID::LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADINGID::LOADING_EDITOR:
		iFlag = pLoading->Loading_ForStage();
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_eID = eLoadingID;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);


	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	switch (m_eID)
	{
#pragma region Stage

	case LOADINGID::LOADING_STAGE:
		//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_PlayerBf", CPlayerBf::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Player_State", CStateMachine::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Player_Stat", CPlayerStat::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeBf", CCubeBf::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeColBf", CCubeColBf::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_BillBoard", CBillBoard::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Animator", CAnimator::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Inventory", CInventory::Create(m_pGraphicDev)), E_FAIL);

		// Monster 
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_State", CStateMachine::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_BasicStat", CBasicStat::Create(m_pGraphicDev)), E_FAIL);

		// Monster Tex
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Warrior", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Warrior/Move/Move%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WarriorAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Warrior/Attack/Attack%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WarriorHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Warrior/Hit/Hit%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WarriorDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Warrior/Dead/Dead%d.png", 2)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Spider", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Surprise/SurpriseSpider/Move/Move%d.png", 12)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SpiderAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Surprise/SurpriseSpider/Attack/Attack%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SpiderHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Surprise/SurpriseSpider/Dead/Dead0.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SpiderDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Surprise/SurpriseSpider/Dead/Dead1.png")), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Bat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Bat/Move/Move%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BatAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Bat/Attack/Attack%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BatHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Bat/Hit/Hit%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BatDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Bat/Dead/Dead%d.png", 4)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Slime", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Slime/Move/Move%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SlimeAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Slime/Attack/Attack%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SlimeHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Slime/Hit/Hit%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SlimeDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Slime/Death/Dead%d.png", 3)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Wizard", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Wizard/Move/Move%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WizardAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Wizard/Attack/Attack%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WizardHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Wizard/Hit/Hit%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WizardDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Wizard/Dead/Dead%d.png", 2)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Alien", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Cave/Alien/Move/Move%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_AlienAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Cave/Alien/Attack/Attack%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_AlienHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Cave/Alien/Hit/Hit%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_AlienDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Cave/Alien/Dead/Dead%d.png", 4)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Skeleton", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BasicSkeleton/Move/Move%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkeletonAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BasicSkeleton/Attack/Attack%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkeletonHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BasicSkeleton/Hit/Hit%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkeletonDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BasicSkeleton/Dead/Dead%d.png", 1)), E_FAIL);
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkeletonReverse", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BasicSkeleton/ReverseHit/Reverse%d.png", 6)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonsterBullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Monster_Bullet/Monster_Bullet%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonsterBulletDelete", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Monster_Bullet/Monster_BulletDelete%d.png", 2)), E_FAIL);

		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Tile/Dungeon/tile%d.png", 25)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Cube/Dungeon/cube%d.dds", 25)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/SkyBox/NightSky%d.dds", 1)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Sword", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Sword/BasicSword0.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Bow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Bow/Bow%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Arrow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Arrow/Arrow.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_RcTexBf", CRcTex::Create(m_pGraphicDev)), E_FAIL);

#pragma region UI
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_HpBarUI",           CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/HPbar/healthbar%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EmptyslotUI",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/emptyslot/emptyslot%d.png", 27)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_itemUI",            CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI%d.png", 41)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_tooltipUI",         CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/tooltip/tooltip%d.png", 43)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_InvenButtonUI",     CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/button/inventory_button%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_playerstatUI",      CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/playerstat.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_mapUI",             CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/map/map%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_aimpointUI",        CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/aimpoint.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NumberUI",          CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Number/%d.png", 16)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EscButtonUI",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/EscUI/Button%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EscBackgroundUI",   CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/EscUI/EscBase.png", 1)), E_FAIL);
		//Test SpeechBubble
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SpeechBubbleUI",	CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/tooltip/tooltip.png")), E_FAIL);
#pragma endregion UI

#pragma region Boss
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Boss",				CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Idle/Boss_Idle%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossAttack",		CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Attack/Boss_Attack%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossHit",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Hit/Hit%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossSleep",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Sleep/Boss_Sleep.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossCrawl",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Crawl/Boss_Crawl%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Boss_StandUp",		CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Stand/Boss_Standing %d.png", 16)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossDying",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Dead/Dead%d.png", 12)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossBatSwarm",		CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Bat_Swarm/Bat_Swarm%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossExplosion",		CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Explosion/Explosion%d.png", 16)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossFire",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Fire/Fire%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossDead",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Wreck/Boss_Wreck.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossChanneling",	CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Channeling/Boss_Channeling%d.png",5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Sturn",				CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Star/Star%d.png", 9)), E_FAIL);
#pragma endregion Boss

#pragma region Collider
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Collider", CCollider::Create(m_pGraphicDev)), E_FAIL);
#pragma endregion Collider

#pragma region NPC
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcTrader", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/ManNPC/Man%d.png", 17)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcAlchemist", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/WayfarerNPC/NPC%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcBard", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/MinstrelNPC/Minstrel%d.png", 22)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcOldMan", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/GrandFaNPC/NPC%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcDogIdle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/DogNPC/DogIdle/DogIdle%d.png", 17)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcDogStand", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/DogNPC/DogStand/DogStand%d.png", 16)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcWizard", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/WizardNPC/Wizard%d.png", 21)), E_FAIL);
#pragma endregion NPC

		break;
#pragma endregion Stage

#pragma region Editor

	case LOADINGID::LOADING_EDITOR:
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeBf", CCubeBf::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Camera", CCamera::Create(m_pGraphicDev)), E_FAIL);

		// Temp : 임시 컴포넌트 -> ???
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Tile/Dungeon/tile%d.png", 25)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/SkyBox/NightSky%d.dds", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Cube/Dungeon/cube%d.dds", 25)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeColBf", CCubeColBf::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Collider", CCollider::Create(m_pGraphicDev)), E_FAIL);

		break;
#pragma endregion Editor
	}
	
	m_bFinish = true;

	return 0;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}

