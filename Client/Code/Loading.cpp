#include "stdafx.h"
#include "Loading.h"
#include "Export_Function.h"
#include "SoundManager.h"

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
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_DynamicBf", CDynamicBf::Create(m_pGraphicDev)), E_FAIL);
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

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkullGhost", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BoneGhost/Move/Move%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkullGhostAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BoneGhost/Attack/Attack%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkullGhostHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BoneGhost/Hit/Hit%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkullGhostDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Undead/BoneGhost/Dead/Dead%d.png", 2)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Worm", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Cave/CaveWorm/Move/Move%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WormAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Cave/CaveWorm/Attack/Attack%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WormHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Cave/CaveWorm/Hit/Hit%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WormDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Cave/CaveWorm/Dead/Dead%d.png", 2)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Monk", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Monk/Move/Move%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonkAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Monk/Attack/Attack%d.png", 9)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonkHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Monk/Hit/Hit%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonkDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Monk/Dead/Dead%d.png", 2)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonsterBullet_Wizard",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Bullet/Monster_Bullet/MonsterBullet_Wizard%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonsterBulletDelete_Wizard", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Bullet/Monster_Bullet/MonsterBulletDelete_Wizard%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonsterBullet_Alien",        CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Bullet/Monster_Bullet/MonsterBullet_Alien%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonsterBulletDelete_Alien",  CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Bullet/Monster_Bullet/MonsterBulletDelete_Alien%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonsterLeafBullet",          CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Bullet/Leaf_bullet/Leaf_bullet%d.png", 3)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_MonsterBulletDelete",        CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Bullet/Monster_Bullet/MonsterBulletDelete%d.png", 2)), E_FAIL);

#pragma region Cube
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Tile", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Tile/Stage/tile%d.png", 52)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Cube/Stage/cube%d.dds", 52)), E_FAIL);
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/SkyBox/NightSky%d.dds", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BladeTrapBody", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Entities/Trap/Blade_Trap_Body.dds")), E_FAIL); //트랩용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_ProjectileTrapBody", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Entities/Trap/Projectile_Trap_Body.dds")), E_FAIL); //트랩용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_PlateTrapBody", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Entities/Trap/Plate_Trap.dds")), E_FAIL); //트랩용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WoodBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Fragile/Box/Box_Cube0.dds")), E_FAIL); //상자용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EquipBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Fragile/Box/Box_Cube1.dds")), E_FAIL); //상자용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Meteor", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Entities/Boss/Meteor.dds")), E_FAIL); //보스용

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Water", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Cube/Stage/water/water%d.dds", 32)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WaterFall", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Cube/Stage/waterfall/waterfall%d.dds", 32)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Lava", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Cube/Stage/lava/lavaCube%d.dds", 32)), E_FAIL);
#pragma endregion Cube

		// Weapon
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Sword", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Sword/BasicSword1.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BowIdle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Bow/BowIDLE.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BowRoming", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Bow/BowRoming%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BowAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Bow/BowAttack.png")), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EpicBowIdle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Bow/EpicBowIDLE.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EpicBowRoming", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Bow/EpicBowRoming%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EpicBowAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Bow/EpicBowAttack%d.png", 5)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Arrow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Arrow/Arrow.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_FireWands", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Wands/Unique2.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_FireBullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Bullet/Sphere_Bullet/Sphere_Bullet%d.png", 8)), E_FAIL);

		// Armor
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Helmet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Armor/Helmet/Helmet2.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Pants", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Armor/Pants/Pants2.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Shield", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Armor/Shield/Shield4.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Shoes", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Armor/Shoes/Shoes1.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Top", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Armor/Top/Top2.png")), E_FAIL);
		

		// Accessories
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Necklace", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI3.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Ring", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI5.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Lamp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI17.png")), E_FAIL);


		// Consumable
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Beer", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI18.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Pot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Fragile/Pot/Pot%d.png", 3)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Bread", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI12.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Apple", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI13.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Cheese", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI14.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Meat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI15.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_RoastMeat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI16.png")), E_FAIL);

		// Potion
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_HolyWater", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI29.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_HpPotion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI24.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_RandomPotion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI27.png")), E_FAIL);

#pragma region UI
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_HpBarUI",           CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/HPbar/healthbar%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EmptyslotUI",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/emptyslot/emptyslot%d.png", 27)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_itemUI",            CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/itemUI%d.png", 42)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_tooltipUI",         CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/tooltip/tooltip%d.png", 43)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_InvenButtonUI",     CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/button/inventory_button%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_playerstatUI",      CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/playerstat.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_mapUI",             CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/map/map%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_aimpointUI",        CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/aimpoint.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NumberUI",          CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Number/%d.png", 18)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EscButtonUI",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/EscUI/Button%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EscBackgroundUI",   CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/EscUI/EscBase.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_ShopUI",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/ShopBackGround.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_ShopFont",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/ShopFont/Font%d.png", 20)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_LevelUpBackground", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Levelup/LevelUpBackGround%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_LevelUpCard",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Levelup/LevelUp%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_LevelUpNumber",     CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Levelup/Number/Number%d.png", 10)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossHpUI",          CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/BossHPbar/BossHPbar%d.png", 2)), E_FAIL);
		//Test SpeechBubble
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SpeechBubbleUI",	CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/tooltip/tooltip43.png")), E_FAIL);
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_PaperUI", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Paper.png", 1)), E_FAIL);
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
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_LostSoul",			CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/LostSoul/LostSoul%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossLightning",		CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/BossLightning.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossCaution", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Caution.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BossWarning", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Boss/Warning.png")), E_FAIL);
#pragma endregion Boss

#pragma region Collider
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Collider", CCollider::Create(m_pGraphicDev)), E_FAIL);
#pragma endregion Collider

#pragma region RigidBody
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_RigidBody", CRigidBody::Create(m_pGraphicDev)), E_FAIL);
#pragma endregion RigidBody

#pragma region NPC
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcTrader", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/ManNPC/Man%d.png", 17)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcAlchemist", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/WayfarerNPC/NPC%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcBard", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/MinstrelNPC/Minstrel%d.png", 22)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcOldMan", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/GrandFaNPC/NPC%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcDogIdle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/DogNPC/DogIdle/DogIdle%d.png", 17)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcDogStand", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/DogNPC/DogStand/DogStand%d.png", 16)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NpcWizard", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/NPC/WizardNPC/Wizard%d.png", 21)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Font", CFont::Create(m_pGraphicDev)), E_FAIL);
#pragma endregion NPC

#pragma region EFFECT
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectWhiteBubble", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/White_bubble/White_bubble%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectBrokenbox",   CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Broken_box/Broken_box%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectDamage",      CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Damage_effect/Damage_effect%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectDamageStar",  CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Star/Star%d.png", 9)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectBlood",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Blood/Blood%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectWand",        CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Pop/Pop%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectExplosion",   CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Explosion/Explosion%d.png", 16)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectTrace",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Trace/Trace%d.png", 11)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectDebuff",      CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Debuff/Debuff%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectPastTrace",   CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Arrow.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectGreenleaves", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Green_leaves/Green_leaves%d.png", 3)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectWaterMove",   CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Water_effect/Water_effect%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EffectWaterBubble", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Water_Bubble/Water_bubble%d.png", 6)), E_FAIL);
#pragma endregion EFFECT
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Shader_Cube",				CShader::Create(m_pGraphicDev, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl"))), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BladeTrapBlade",	CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Trap/Trap_Blade.png")), E_FAIL); //트랩용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Torch",				CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Immortal/Torch/Torch%d.png",8)), E_FAIL);

#pragma region SOUND
		// 테스트용

		//MONSTER
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_spider_idle_01.mp3", "en_spider_idle_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_spider_attack_01.mp3", "en_spider_attack_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_spider_die_01.mp3", "en_spider_die_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_spider_hurt_01.mp3", "en_spider_hurt_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/spider_walk.mp3", "spider_walk.mp3"), E_FAIL);

		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_melee_2_alert_02.mp3", "en_melee_2_alert_02.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_melee_2_attack_02.mp3", "en_melee_2_attack_02.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_melee_2_die_02.mp3", "en_melee_2_die_02.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_melee_2_hurt_02.mp3", "en_melee_2_hurt_02.mp3"), E_FAIL);

		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_bat_alert_01.mp3", "en_bat_alert_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_bat_attack_02.mp3", "en_bat_attack_02.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_bat_death_01.mp3", "en_bat_death_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_bat_hurt_01.mp3", "en_bat_hurt_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_bat_idle_01.mp3", "en_bat_idle_01.mp3"), E_FAIL);

		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_mage_alert_03.mp3", "en_mage_alert_03.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_mage_attack_01.mp3", "en_mage_attack_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_mage_die_01.mp3", "en_mage_die_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_mage_hurt_03.mp3", "en_mage_hurt_03.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_mage_idle_01.mp3", "en_mage_idle_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/mg_fire_shoot_03.mp3", "mg_fire_shoot_03.mp3"), E_FAIL);

		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_slime_alert_02.mp3", "en_slime_alert_02.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_slime_attack_01.mp3", "en_slime_attack_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_slime_die_03.mp3", "en_slime_die_03.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_slime_hurt_01.mp3", "en_slime_hurt_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_slime_idle_03.mp3", "en_slime_idle_03.mp3"), E_FAIL);

		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_beholder_alert_01.mp3", "en_beholder_alert_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_beholder_attack_01.mp3", "en_beholder_attack_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_beholder_die_01.mp3", "en_beholder_die_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_beholder_hurt_01.mp3", "en_beholder_hurt_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_beholder_idle_01.mp3", "en_beholder_idle_01.mp3"), E_FAIL);

		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_worm_alert_03.mp3", "en_worm_alert_03.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_worm_idle_03.mp3", "en_worm_idle_03.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_worm_attack_02.mp3", "en_worm_attack_02.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_worm_die_01.mp3", "en_worm_die_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_worm_hurt_02.mp3", "en_worm_hurt_02.mp3"), E_FAIL);

		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_skel_idle_01.mp3", "en_skel_idle_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_skel_attack_01.mp3", "en_skel_attack_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_skel_death_01.mp3", "en_skel_death_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_skel_hurt_02.mp3", "en_skel_hurt_02.mp3"), E_FAIL);

		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_skull_idle_01.mp3", "en_skull_idle_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_skull_atk_01.mp3", "en_skull_atk_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_skull_die_01", "en_skull_die_01"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/en_skull_hurt_03.mp3", "en_skull_hurt_03.mp3"), E_FAIL);

		//PLAYER
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/feet_default_01.mp3", "feet_default_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/feet_wood_01.mp3", "feet_wood_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/attack_04.mp3", "attack_04.mp3"), E_FAIL);


		//WEAPON
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/bow.wav", "bow.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/whoosh1.mp3", "whoosh1.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/explode.mp3", "explode.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/wep_swing_bow_load_01.mp3", "wep_swing_bow_load_01.mp3"), E_FAIL);


		// SceneBGM
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/campfire-guitar.mp3", "campfire-guitar.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/chase_sewers.mp3", "chase_sewers.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/door_beginning.mp3", "door_beginning.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/env_waterfall.mp3", "env_waterfall.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/04_swords_and_skulls.mp3", "04_swords_and_skulls.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/DK-7.mp3", "DK-7.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/MyDoor.mp3", "MyDoor.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Test_BossTheme.mp3", "Test_BossTheme.mp3"), E_FAIL);

		// Inventory
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/grab_item.mp3", "grab_item.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/open_inventory.mp3", "open_inventory.mp3"), E_FAIL);

		// Collision && Break 
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/break_wood_01.mp3", "break_wood_01.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/torch.mp3", "torch.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/clang_02.mp3", "clang_02.mp3"), E_FAIL);

		//UI && INVENTORY
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/ui_dialogue_open.mp3", "ui_dialogue_open.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/ui_dialogue_close.mp3", "ui_dialogue_close.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/ui_buy.mp3", "ui_buy.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/ui_equip_armor.mp3", "ui_equip_armor.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/ui_equip_item.mp3", "ui_equip_item.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/ui_equip_weapon.mp3", "ui_equip_weapon.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/cons_drink.mp3", "cons_drink.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/cons_food.mp3", "cons_food.mp3"), E_FAIL);

		//HekiRekiIssen
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Production/HekiRekiIssen.mp3", "HekiRekiIssen"), E_FAIL);
		//Boss
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Attack1.wav", "Boss_Attack1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Attack2.wav", "Boss_Attack2.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Fire1.wav", "Boss_Fire1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Fire2.wav", "Boss_Fire2.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Fire3.wav", "Boss_Fire3.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_FireBall1.wav", "Boss_FireBall1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_FireWall1.wav", "Boss_FireWall1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_FireWave1.wav", "Boss_FireWave1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Laugh1.wav", "Boss_Laugh1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Laugh2.wav", "Boss_Laugh2.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_LowHp.wav", "Boss_LowHp.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Meteor1.wav", "Boss_Meteor1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_MeteorExplosion1.wav", "Boss_MeteorExplosion1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_MeteorExplosion2.wav", "Boss_MeteorExplosion2.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_MeteorExplosion3.wav", "Boss_MeteorExplosion3.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_MiniMeteor.mp3", "Boss_MiniMeteor.mp3"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Taunt1.wav", "Boss_Taunt1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Boss_Taunt2.wav", "Boss_Taunt2.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Elec1.wav", "Elec1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/LostSoul1.wav", "LostSoul1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/LostSoul2.wav", "LostSoul2.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/LostSoul3.wav", "LostSoul3.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/LostSoulSpawn1.wav", "LostSoulSpawn1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/LostSoulSpawn2.wav", "LostSoulSpawn2.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Parry1.wav", "Parry1.wav"), E_FAIL);
		FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/Parry2.wav", "Parry2.wav"), E_FAIL);
		//FAILED_CHECK_RETURN(CSoundManager::GetInstance()->LoadSound("../Bin/SRSource/Sound/Boss/.wav", ".wav"), E_FAIL);
#pragma endregion SOUND

#pragma region VILLAGE
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageSkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE,   L"../Bin/SRSource/Village/Sky/VillageSky.dds", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Moon",          CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Sky/Moon.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Tree",          CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Tree/Tree%d.png", 24)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Star",          CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Sky/Star%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Bonfire",       CTexture::Create(m_pGraphicDev, TEX_CUBE,   L"../Bin/SRSource/Village/Bonfire/BonfireCube%d.dds", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Tomb",          CTexture::Create(m_pGraphicDev, TEX_CUBE,   L"../Bin/SRSource/Village/Tomb/TombCube.dds", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_FireEffect",    CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Effect/Square_effect/Square_effect_Yellow2.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageFire",   CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Fire/Fire%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageTiger",  CTexture::Create(m_pGraphicDev, TEX_CUBE,   L"../Bin/SRSource/Village/Triger/TrigerCube.dds", 1)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageRock",     CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Rock/Rock%d.png", 3)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageGrass",    CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Grass/Grass%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageMushroom", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Mushroom/Mushroom%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillagePumpkin",  CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Pumpkin/Pumpkin0.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Firefly",         CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Firefly.png", 1)), E_FAIL);
#pragma endregion VILLAGE

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_ImmortalSprite", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Immortal/Tile/Tile%d.png", 20)), E_FAIL);
		break;
#pragma endregion Stage

#pragma region Editor

	case LOADINGID::LOADING_EDITOR:
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeBf", CCubeBf::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Camera", CCamera::Create(m_pGraphicDev)), E_FAIL);

		// Temp : 임시 컴포넌트 -> ???
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Tile", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Tile/Stage/tile%d.png", 52)), E_FAIL);
		//FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/SkyBox/NightSky%d.dds", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Cube/Stage/cube%d.dds", 52)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeColBf", CCubeColBf::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Collider", CCollider::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_RigidBody", CRigidBody::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Animator", CAnimator::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_State", CStateMachine::Create(m_pGraphicDev)), E_FAIL);

#pragma region Trap
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BladeTrapBlade", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Trap/Trap_Blade.png")), E_FAIL); //트랩용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Torch", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Immortal/Torch/Torch%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_BladeTrapBody", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Entities/Trap/Blade_Trap_Body.dds")), E_FAIL); //트랩용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_ProjectileTrapBody", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Entities/Trap/Projectile_Trap_Body.dds")), E_FAIL); //트랩용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_PlateTrapBody", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Entities/Trap/Plate_Trap.dds")), E_FAIL); //트랩용
#pragma endregion Trap

#pragma region Immortal
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Tree", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Tree/Tree%d.png", 24)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageRock", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Rock/Rock%d.png", 3)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageGrass", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Grass/Grass%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillageMushroom", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Mushroom/Mushroom%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_VillagePumpkin", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Village/Pumpkin/Pumpkin0.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_ImmortalSprite", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Immortal/Tile/Tile%d.png", 20)), E_FAIL);
#pragma endregion Immortal

#pragma region MyRegion
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_WoodBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Fragile/Box/Box_Cube0.dds")), E_FAIL); //상자용
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EquipBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/SRSource/Fragile/Box/Box_Cube1.dds")), E_FAIL); //상자용
#pragma endregion MyRegion

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

