#include "stdafx.h"
#include "..\Header\Loading.h"
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
	case LOADINGID::LOADING_STAGE:
		//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_PlayerBf", CPlayerBf::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeBf", CCubeBf::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_BillBoard", CBillBoard::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Warrior_AI", CWarriorAI::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Monster_AI", CMonsterAI::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);


		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Sword", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Equip/Weapon/Sword/BasicSword0.png")), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_RcTexBf", CRcTex::Create(m_pGraphicDev)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Warrior", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Dungeon/Warrior/Move/Move%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Spider", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/Entities/Surprise/SurpriseSpider/Move/Move%d.png", 12)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_HpBarUI",           CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/HPbar/healthbar%d.png", 3)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_EmptyslotUI",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/emptyslot/emptyslot%d.png", 20)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_itemUI_equipment",  CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/equipment%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_itemUI_food",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/food%d.png", 5)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_itemUI_item",       CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/item%d.png", 6)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_itemUI_potion",     CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/potion%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_itemUI_weaponitem", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/item/weaponitem%d.png", 11)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_tooltipUI",         CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/tooltip/tooltip.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_InvenButtonUI",     CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/button/inventory_button%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_playerstatUI",      CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/playerstat.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_mapUI",             CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Inventory/map/map%d.png", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_aimpointUI",        CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/aimpoint.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_NumberUI",          CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/Number/%d.png", 11)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_HalfNumUI",         CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/SRSource/UI/HalfNumber/%d.png", 5)), E_FAIL);

		break;

	case LOADINGID::LOADING_EDITOR:
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_CubeBf", CCubeBf::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);

		// Temp : 임시 컴포넌트
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);

		break;
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

