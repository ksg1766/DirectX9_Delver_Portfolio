#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"
#include "PoolManager.h"

#include "Monstergroup.h"
#include "Itemgroup.h"
#include "Npc_Trader.h"
#include "UIUseShop_Trander.h"
#include "UIShop.h"

#include "SpawningPool.h"
#include "Box_Cube.h"
#include "EffectSquare.h"
#include "EffectBubble.h"

#include "Blade_Trap_Body.h"
#include "StrikeDown_Trap_Body.h"
#include "Plate_Trap_Body.h"
#include "Pot.h"

#include "EffectBrokenbox.h"
#include "EffectDamage.h"
#include "EffectDamageStar.h"
#include "EffectBlood.h"
#include "EffectProjectileTrace.h"
#include "EffectTwinkle.h"
#include "SoundManager.h"

#include "Jump_Plate.h"
#include "BlackIn.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	Engine::CGameObject* pGameObject = CBlackIn::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_BASIC, 0);
	Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_UP, pGameObject);

	m_eSceneTag = SCENETAG::STAGE;

	//FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);

	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{
	__super::Update_Scene(fTimeDelta);

	UIManager()->Update_UI(fTimeDelta);

	return 0;
}

void CStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

	CollisionManager()->LateUpdate_Collision();
	UIManager()->LateUpdate_UI();

	// 테스트용입니다.
	//if (Engine::InputDev()->Key_Down(DIK_F7))
	//{
	//	CGameObject* pGameObject = CEffectBubble::Create(m_pGraphicDev);

	//	//pGameObject->m_pTransform->Translate(_vec3(-40.f, 5.f, -40.f));
	//	//Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	//	CPoolManager::GetInstance()->Create_Effect(EFFECTTAG::EFFECT_BROKENBOX, _vec3(-40.f, 5.f, -40.f));
	//}
	//else if (Engine::InputDev()->Key_Down(DIK_F8))
	//{
	//	CGameObject* pGameObject = CEffectBrokenbox::Create(m_pGraphicDev);
	//	pGameObject->m_pTransform->Translate(_vec3(-40.f, 3.5f, -40.f));
	//	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	//	//CPoolManager::GetInstance()->Create_Effect(EFFECTTAG::, TargetPos);
	//}

	//if (Engine::InputDev()->Key_Down(DIK_F9))
	//{
	//	CSoundManager::GetInstance()->PlaySound(L"01_Title_Screen", CHANNELID::SOUND_EFFECT, g_fSound);
	//	CSoundManager::GetInstance()->PlayBGM(L"11_10.Player_Die.wav", g_fSound);
	//}
	//if (Engine::InputDev()->Key_Pressing(DIK_K))
	//{
	//	CSoundManager::GetInstance()->PlaySound(L"03_StartVillage", CHANNELID::SOUND_EFFECT, g_fSound);
	//	CSoundManager::GetInstance()->PlayBGM(L"11_10.Player_Die.wav", g_fSound);
	//}

}

void CStage::Render_Scene()
{
	// DEBUG 용 렌더
}

void CStage::Free()
{
	CPoolManager::DestroyInstance();

	__super::Free();
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CStage::Ready_Prototype()
{

	return S_OK;
}

HRESULT CStage::Ready_Layer_Environment(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

#pragma region 첫 번째 씬에서 받아오는 오브젝트
	// DynamicCamera
	//pGameObject = CDynamicCamera::Create(m_pGraphicDev, 
	//										&_vec3(0.f, 0.f, 0.f),
	//										&_vec3(0.f, 0.f, 1.f),
	//										&_vec3(0.f, 1.f, 0.f),
	//										D3DXToRadian(90.f), 
	//										(_float)WINCX / WINCY,
	//										0.1f, 
	//										1000.f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
#pragma endregion 첫 번째 씬에서 받아오는 오브젝트

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// Test SpawningPool
	/*pGameObject = CSpawningPool::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-50.f, 0.f, -50.f));
	dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnTime(5.f);
	dynamic_cast<CSpawningPool*>(pGameObject)->Set_MonsterTag(MONSTERTAG::WIZARD);
	dynamic_cast<CSpawningPool*>(pGameObject)->Set_PoolCapacity(7);
	dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnRadius(30.f);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);*/

	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	CPoolManager::GetInstance()->Ready_Pool();

	Engine::CGameObject*		pGameObject = nullptr;

#pragma region 첫 번째 씬에서 받아오는 오브젝트
	//// Player
    //pGameObject = CPlayer::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    
    ////pGameObject->m_pTransform->Translate(_vec3(-40.f, 1.f,-40.f));
    //pGameObject->m_pTransform->Translate(_vec3(0.f, 1.f, 0.f));
    ////pGameObject->m_pTransform->Translate(_vec3(100.f, 10.f,0.f));
    
    //pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
    //m_pPlayer = dynamic_cast<CPlayer*>(pGameObject);
    
    //// FootRay
    //pGameObject = CFootRay::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
    //dynamic_cast<CFootRay*>(pGameObject)->Set_Host(m_pPlayer);
    ////pGameObject->m_pTransform->Translate(m_pPlayer->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -1.25f, 0.f));

#pragma endregion 첫 번째 씬에서 받아오는 오브젝트

	// Boss
	pGameObject = CSkeletonKing::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-72.5f, 34.f, 94.5f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CJump_Plate::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-2.f, 11.5f, -2.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CBlade_Trap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-3.f, 11.5f, -3.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CBlade_Trap*>(pGameObject)->Create_Blade();

	pGameObject = CStrikeDown_Trap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-10.f, 30.f, 15.f));
	dynamic_cast<CStrikeDown_Trap*>(pGameObject)->Set_InitailHeight(20.f);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// Npc Test
	pGameObject = CNpc_OldMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-45.f, 1.f, -40.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CNpc_Bard::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-55.f, 1.f, -55.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CNpc_Trader::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-40.f, 1.f, -35.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	CItem* pItem = CTempItem::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40, 1.5f, 0.f));
	dynamic_cast<CTempItem*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	pItem = CEpicBow::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-45, 1.5f, -35.f));
	dynamic_cast<CEpicBow*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	
	pItem = CBow::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40.f, 1.5f, 5.f));
	dynamic_cast<CBow*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	
	pItem = CFireWands::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40.f, 1.5f, 10.f));
	dynamic_cast<CFireWands*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	pItem = CHelmet::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40, 3.5f, -35.f));
	dynamic_cast<CHelmet*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	pItem = CTop::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40, 3.5f, -30.f));
	dynamic_cast<CTop*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	pItem = CPants::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40, 3.5f, -25.f));
	dynamic_cast<CPants*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	pItem = CShield::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(-40, 3.5f, -20.f));
	pItem->m_pTransform->Translate(_vec3(100.f, 20.f, 0.f));
	dynamic_cast<CShield*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	pItem = CNecklace::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40, 3.5f, -15.f));
	dynamic_cast<CNecklace*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	pItem = CRing::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40, 3.5f, -10.f));
	dynamic_cast<CRing*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	pItem = CBread::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pItem, E_FAIL);
	pItem->m_pTransform->Translate(_vec3(-40, 3.5f, -5.f));
	dynamic_cast<CBread*>(pItem)->Set_WorldItem(true);
	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	//pGameObject = CWorm::Create(m_pGraphicDev);
	//pGameObject->m_pTransform->Translate(_vec3(-30.f, 3.f, -40.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	//pItem = CApple::Create(m_pGraphicDev, true);
	//NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(-40, 3.5f, 0.f));
	//dynamic_cast<CApple*>(pItem)->Set_WorldItem(true);
	//pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	//
	//pItem = CCheese::Create(m_pGraphicDev, true);
	//NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(-40, 3.5f, 5.f));
	//dynamic_cast<CCheese*>(pItem)->Set_WorldItem(true);
	//pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	//
	//pGameObject = CBox_Cube::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(-30.f, 3.5f, -30.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	//pItem = CMeat::Create(m_pGraphicDev, true);
	//NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(-40, 3.5f, 10.f));
	//dynamic_cast<CMeat*>(pItem)->Set_WorldItem(true);
	//pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	//
	//pItem = CRoastmeat::Create(m_pGraphicDev, true);
	//NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(-40, 3.5f, 15.f));
	//dynamic_cast<CRoastmeat*>(pItem)->Set_WorldItem(true);
	//pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	//
	//pItem = CHolyWater::Create(m_pGraphicDev, true);
	//NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(-45, 3.5f, -35.f));
	//dynamic_cast<CHolyWater*>(pItem)->Set_WorldItem(true);
	//pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	//
	//pItem = CHpPotion::Create(m_pGraphicDev, true);
	//NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(-45, 3.5f, -30.f));
	//dynamic_cast<CHpPotion*>(pItem)->Set_WorldItem(true);
	//pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	//
	//pItem = CLamp::Create(m_pGraphicDev, true);
	//NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(-50, 3.5f, -30.f));
	//dynamic_cast<CLamp*>(pItem)->Set_WorldItem(true);
	//pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	//for(_int i = 0; i < 5; ++i)
	//{
	//	pItem = CRandomPotion::Create(m_pGraphicDev, true);
	//	NULL_CHECK_RETURN(pItem, E_FAIL);
	//	pItem->m_pTransform->Translate(_vec3(-45, 3.5f, -25.f + (i*5)));
	//	dynamic_cast<CRandomPotion*>(pItem)->Set_WorldItem(true);
	//	pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);
	//}


	pGameObject = CBlade_Trap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(95.f, 20.f, 0.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CBlade_Trap*>(pGameObject)->Create_Blade();

	pGameObject = CStrikeDown_Trap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(95.f, 25.f, 0.f));
	dynamic_cast<CStrikeDown_Trap*>(pGameObject)->Set_InitailHeight(10.f);
	dynamic_cast<CStrikeDown_Trap*>(pGameObject)->Set_MinHeight(0.f);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	//pGameObject = CPlate_Trap::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(-40.f, 3.5f, -40.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	//pGameObject = CWorm::Create(m_pGraphicDev);
	//pGameObject->m_pTransform->Translate(_vec3(-30.f, 3.f, -40.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	//pGameObject = CBox_Cube::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(-30.f, 3.5f, -30.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	//pGameObject = CPot::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(-25.f, 4.f, -25.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	//pGameObject = CBoneGhost::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(-70.f, 1.f, -40.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(LAYERTAG _eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

#pragma region 첫 번째 씬에서 받아오는 오브젝트
	// 기본 인벤토리 5칸
	for (_uint i = 0; i < 5; ++i)
	{
		pGameObject = CUIbasicslot::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 520.f + (60.f * i);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].y = WINCY - 30.f;
		dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
		dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_SLOTBASIC, i);
		dynamic_cast<CTempUI*>(pGameObject)->Set_Setup(true);
		Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);
	}

	_uint Index = 0;
	// 장비슬롯
	for (_uint iy = 0; iy < 3; ++iy)
	{
		for (_uint ix = 0; ix < 2; ++ix)
		{

			Index = iy * 2 + ix;
			pGameObject = CUIequipmentslot::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 350.f + (60.f * ix);
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = WINCY - 135.f - (60.f * iy);
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_SLOTEQUIPMENT, Index);
			dynamic_cast<CTempUI*>(pGameObject)->Set_Setup(true);
			Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
		}
	}

	// 메인 인벤토리
	for (_uint iy = 0; iy < 3; ++iy)
	{
		for (_uint ix = 0; ix < 6; ++ix)
		{

			pGameObject = CUIemptyslot::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 490.f + (60.f * ix);
			pGameObject->m_pTransform->m_vInfo[INFO_POS].y = WINCY - 135.f - (60.f * iy);
			dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_SLOTEMPTY, iy * 6 + ix);
			dynamic_cast<CTempUI*>(pGameObject)->Set_Setup(true);
			Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_INVEN, Engine::UILAYER::UI_DOWN, pGameObject);
		}
	}

	// 인벤토리 및 스탯 버튼
	for (_uint iy = 0; iy < 2; ++iy)
	{
		pGameObject = CUIbutton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 850.f;
		pGameObject->m_pTransform->m_vInfo[INFO_POS].y = 595.f - (40 * iy);
		dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
		dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_INVENBUTTON, iy);
		if (iy == 0)
		{
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIImage(1);
		}
		else
		{
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIImage(3);
		}

		Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
	}

	pGameObject = CUIplayerstat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_STAT, Engine::UILAYER::UI_DOWN, pGameObject);

	// 조준점
	HCURSOR Cursor = nullptr;
	SetCursor(Cursor);
	pGameObject = CUIaimpoint::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_MOUSE, Engine::UILAYER::UI_DOWN, pGameObject);

	// 플레이어 hp bar
	pGameObject = CUIplayerhp::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);

	// 큰 지도
	pGameObject = CUIbigmap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_MAP, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc 배경
	pGameObject = CUIEscBackground::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Resum 버튼
	pGameObject = CUIResumeButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Option 버튼
	pGameObject = CUIOptionButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Esc Quition 버튼
	pGameObject = CUIQuitButton::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

	// Shop
	pGameObject = CUIShop::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_SHOP, Engine::UILAYER::UI_DOWN, pGameObject);


	// Speech Bubble Test
	pGameObject = CUIspeech_OldMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

	pGameObject = CUIUseShop_Trander::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_INVEN);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_STAT);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_MAP);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_ESC);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_SPEECH);	// Speech Bubble Test
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_SHOP);	// Speech Bubble Test
#pragma endregion 첫 번째 씬에서 받아오는 오브젝트
	
	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Load_Data()
{
	CLayer* pLayer = m_mapLayer[LAYERTAG::GAMELOGIC];
	for (int i = 0; i < (UINT)OBJECTTAG::OBJECT_END; ++i)
	{
		// 일단 블록만
		if (OBJECTTAG::BLOCK != (OBJECTTAG)i)
			continue;

		vector<CGameObject*>& refObjectList = pLayer->Get_ObjectList((OBJECTTAG)i);
		for_each(refObjectList.begin(), refObjectList.end(), [&](CGameObject* pObj) { EventManager()->DeleteObject(pObj); });
		refObjectList.clear();
	}
	//HANDLE hFile = CreateFile(L"../Bin/Data/Sewer_TrapTest.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(L"../Bin/Data/Sewer.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(L"../Bin/Data/TempData.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE hFile = CreateFile(L"../Bin/Data/Sewer.dat", GENERIC_READ,	0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(L"../Bin/Data/BossStage_3rd.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	OBJECTTAG eTag = OBJECTTAG::OBJECT_END;

	DWORD		dwByte = 0;
	_float		fX, fY, fZ;
	_ubyte		byTextureNumber = 0;

	MONSTERTAG	eSpawnerTag = MONSTERTAG::MONSTER_END;  //
	_int		iPoolCapacity = 5;
	_float		fSpawnRadius = 10.0f;
	_float		fSpawnTime = 10.0f;

	while (true)
	{
		// key 값 저장
		ReadFile(hFile, &eTag, sizeof(OBJECTTAG), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		// if문 추가
		if (OBJECTTAG::BLOCK == eTag)
		{
			// value값 저장
			ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

			ReadFile(hFile, &byTextureNumber, sizeof(_ubyte), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject* pGameObject = nullptr;

			if (31 == byTextureNumber || 32 == byTextureNumber)
			{
				pGameObject = CDynamicCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				dynamic_cast<CDynamicCubeBlock*>(pGameObject)->Set_TextureNumber(byTextureNumber);
				pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
				pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
				//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
			else
			{
				pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				dynamic_cast<CCubeBlock*>(pGameObject)->Set_TextureNumber(byTextureNumber);
				pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
				pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
				//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
		}
		else if (OBJECTTAG::SPAWNINGPOOL == eTag)
		{
			// value값 저장
			ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

			ReadFile(hFile, &eSpawnerTag, sizeof(MONSTERTAG), &dwByte, nullptr);
			ReadFile(hFile, &iPoolCapacity, sizeof(_int), &dwByte, nullptr);
			ReadFile(hFile, &fSpawnRadius, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fSpawnTime, sizeof(_float), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject* pGameObject = CSpawningPool::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CSpawningPool*>(pGameObject)->Set_MonsterTag(eSpawnerTag);
			dynamic_cast<CSpawningPool*>(pGameObject)->Set_PoolCapacity(iPoolCapacity);
			dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnRadius(fSpawnRadius);
			dynamic_cast<CSpawningPool*>(pGameObject)->Set_SpawnTime(fSpawnTime);
			pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
			pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
			//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		else if (OBJECTTAG::MONSTER == eTag)
		{
			// value값 저장
			ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

			TRAPTAG eTrapTag;
			ReadFile(hFile, &eTrapTag, sizeof(TRAPTAG), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject* pGameObject = nullptr;

			switch (eTrapTag)
			{
			case TRAPTAG::BLADE:
				pGameObject = CBlade_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CBlade_Trap*>(pGameObject)->Create_Blade();
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;

			case TRAPTAG::STRIKEDOWN:
				pGameObject = CStrikeDown_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CStrikeDown_Trap*>(pGameObject)->Set_InitailHeight(15.f);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;

			case TRAPTAG::PLATE:
				pGameObject = CPlate_Trap::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				break;
			}

			pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(fX, fY + 1.f, fZ);
			pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
			//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
	}
	CloseHandle(hFile);
	return S_OK;
}
