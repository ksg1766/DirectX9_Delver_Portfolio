#include "stdafx.h"
#include "..\Header\Village.h"

#include "Export_Function.h"
#include "PoolManager.h"

#include "Monstergroup.h"
#include "Itemgroup.h"
#include "NpcGroup.h"
#include "SoundManager.h"

#include "SpawningPool.h"
#include "BoxCube.h"
#include "EquipBox.h"
#include "DoorCube.h"
#include "UIPuzzleBack.h"

#include "Blade_Trap_Body.h"
#include "StrikeDown_Trap_Body.h"
#include "Plate_Trap_Body.h"
#include "Pot.h"
#include "SpiderRay.h"

#include "SkyBoxVillage.h"
#include "Moon.h"
#include "Tree.h"
#include "EffectStar.h"
#include "Bonfire.h"
#include "EffectBonfire.h"
#include "Fire.h"
#include "VillageTriger.h"
#include "BlackIn.h"
#include "Rock.h"
#include "Grass.h"
#include "Mushroom.h"
#include "Pumpkin.h"
#include "ImmortalSprite.h"
#include "EffectFallingleaves.h"
#include "EffectFirefly.h"
#include "HellDoor.h"

// 연출 테스트 // 성공시 보스 씬으로 이동
#include "GameManager.h"
#include "FlyingCamera.h"
#include "OrthoCamera.h"
#include "CameraManager.h"

#include "UILevelUp.h"
#include "UILevelUpCard.h"
#include "UIbosshp.h"
#include "EffectWaterfall.h"
#include <EffectWaterMove.h>

CVillage::CVillage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CVillage::~CVillage()
{
}

HRESULT CVillage::Ready_Scene()
{
	Engine::CGameObject* pGameObject = CBlackIn::Create(m_pGraphicDev);
	dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_BASIC, 0);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_BLACK, Engine::UILAYER::UI_DOWN, pGameObject);

	m_eSceneTag = SCENETAG::VILLAGE;

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);

	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	CSoundManager::GetInstance()->StopAll();
	CSoundManager::GetInstance()->PlayBGM(L"campfire-guitar.mp3", 0.5f);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	return S_OK;
}

Engine::_int CVillage::Update_Scene(const _float& fTimeDelta)
{
	__super::Update_Scene(fTimeDelta);

	CGameManager::GetInstance()->Update_Game(fTimeDelta);
	CCameraManager::GetInstance()->Update_Camera(fTimeDelta);

	UIManager()->Update_UI(fTimeDelta);
	return 0;
}

void CVillage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

	CollisionManager()->LateUpdate_Collision();
	CCameraManager::GetInstance()->LateUpdate_Camera();

	UIManager()->LateUpdate_UI();
}

void CVillage::Render_Scene()
{
	// DEBUG 용 렌더
}

HRESULT CVillage::Ready_Layer_Environment(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	Engine::CGameObject*		pGameObject = nullptr;

	if (!Engine::SceneManager()->Get_VisitScene(m_eSceneTag))
	{
		// DynamicCamera
		/*pGameObject = CDynamicCamera::Create(m_pGraphicDev,
			&_vec3(0.f, 0.f, 0.f),
			&_vec3(0.f, 0.f, 1.f),
			&_vec3(0.f, 1.f, 0.f),
			D3DXToRadian(90.f),
			(_float)WINCX / WINCY,
			0.1f,
			200.f);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);*/

		// FlyingCamera
		pGameObject = CFlyingCamera::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

		// OrthoCamera
		pGameObject = COrthoCamera::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	}
	
	// SkyBox
	pGameObject = CSkyBoxVillage::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// Moon
	pGameObject = CMoon::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// Star
	for (_uint i = 0; i < 50; ++i)
	{
		pGameObject = CEffectStar::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	}

	// 모닥불
	pGameObject = CBonfire::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	pGameObject = CFire::Create(m_pGraphicDev);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// 불 이펙트 추가 생성
	for (_uint i = 0; i < 4; ++i) {
		pGameObject = CEffectBonfire::Create(m_pGraphicDev);
		pGameObject->m_pTransform->Translate(_vec3(0.f, 1.f * i, 0.f));
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	}

	// 씬 이동 트리거
	pGameObject = CVillageTriger::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// 나뭇잎 이펙트
	pGameObject = CEffectFallingleaves::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// 반딧불이 이펙트
	for (_uint i = 0; i < 50; ++i)
	{
		pGameObject = CEffectFirefly::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	}

	// 폭포 이펙트
	pGameObject = CEffectWaterfall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-23.f, 1.f, - 71.f)); 
	dynamic_cast<CEffectWaterfall*>(pGameObject)->Set_BoundingBox(_vec3(-2.f, 4.f, -1.f), _vec3(2.f, 5.f, 1.f));
	dynamic_cast<CEffectWaterfall*>(pGameObject)->Set_EffectMoveScale(5, _vec3(.2f, .2f, .2f), _vec3(1.5f, 1.5f, 1.5f));
	dynamic_cast<CEffectWaterfall*>(pGameObject)->Set_EffectBubbleScale(7, _vec3(.2f, .2f, .2f), _vec3(1.f, 1.f, 1.f));
	dynamic_cast<CEffectWaterfall*>(pGameObject)->Set_EffectDropHeight(1.f);
	dynamic_cast<CEffectWaterfall*>(pGameObject)->Set_EffectMoveSet(1);
;	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

#pragma region TREE
	// 시작 시 왼쪽 그룹
	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(0, 0);
	pGameObject->m_pTransform->Scale(_vec3(9.f, 9.f, 9.f));
	pGameObject->m_pTransform->Translate(_vec3(-8.f, 12.f, -62.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(1, 1);
	pGameObject->m_pTransform->Scale(_vec3(8.f, 8.f, 8.f));
	pGameObject->m_pTransform->Translate(_vec3(-12.5f, 11.f, -66.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(2, 3);
	pGameObject->m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
	pGameObject->m_pTransform->Translate(_vec3(-7.f, 5.f, -65.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(0, 3);
	pGameObject->m_pTransform->Scale(_vec3(2.f, 2.f, 2.f));
	pGameObject->m_pTransform->Translate(_vec3(-6.f, 4.f, -67.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(0, 3);
	pGameObject->m_pTransform->Scale(_vec3(2.5f, 2.5f, 2.5f));
	pGameObject->m_pTransform->Translate(_vec3(-16.f, 4.5f, -68.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// 시작 시 오른쪽 그룹
	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(1, 1);
	pGameObject->m_pTransform->Scale(_vec3(7.f, 7.f, 7.f));
	pGameObject->m_pTransform->Translate(_vec3(10.f, 10.f, -64.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(0, 0);
	pGameObject->m_pTransform->Scale(_vec3(8.f, 8.f, 8.f));
	pGameObject->m_pTransform->Translate(_vec3(15.f, 11.f, -67.5f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(0, 3);
	pGameObject->m_pTransform->Scale(_vec3(5.f, 5.f, 5.f));
	pGameObject->m_pTransform->Translate(_vec3(21.f, 6.f, -65.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CTree::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTree*>(pGameObject)->Set_TreeType(2, 3);
	pGameObject->m_pTransform->Scale(_vec3(3.f, 3.f, 3.f));
	pGameObject->m_pTransform->Translate(_vec3(28.f, 5.f, -62.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
#pragma endregion TREE

	return S_OK;
}

HRESULT CVillage::Ready_Layer_GameLogic(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	CPoolManager::GetInstance()->Ready_Pool();

	Engine::CGameObject*		pGameObject = nullptr;

	if (!Engine::SceneManager()->Get_VisitScene(m_eSceneTag))
	{
		// Player
		pGameObject = CPlayer::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->m_pTransform->Translate(_vec3(0.f, 0.f, -70.f));
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
		m_pPlayer = dynamic_cast<CPlayer*>(pGameObject);

		// FootRay
		pGameObject = CFootRay::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
		dynamic_cast<CFootRay*>(pGameObject)->Set_Host(m_pPlayer);
		//pGameObject->m_pTransform->Translate(m_pPlayer->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -1.25f, 0.f));
	}

	// EpicWeapon
	//CItem* pItem = CEpicBow::Create(m_pGraphicDev, true);
	//NULL_CHECK_RETURN(pItem, E_FAIL);
	//pItem->m_pTransform->Translate(_vec3(0, 1.5f, -22.f));
	//dynamic_cast<CEpicBow*>(pItem)->Set_WorldItem(true);
	//pLayer->Add_GameObject(pItem->Get_ObjectTag(), pItem);

	for (_uint i = 0; i < 5; ++i)
	{
		pGameObject = CBoxCube::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		_vec3 vPos = _vec3(-30.f, 3.5f, -30.f - (i * 5));
		pGameObject->m_pTransform->Translate(vPos);
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	}

	for (_uint i = 0; i < 5; ++i)
	{
		pGameObject = CEquipBox::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		_vec3 vPos = _vec3(-30.f, 3.5f, -30.f + (i * 5));
		pGameObject->m_pTransform->Translate(vPos);
		pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	}


	// Npc Test
	pGameObject = CNpc_OldMan::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-5.f, 1.f, -22.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CNpc_Bard::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(5.f, 1.f, -22.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CNpc_Trader::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(3.f, 1.f, -18.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CNpc_Dog::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-3.f, 1.f, -18.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CNpc_Alchemist::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-3.f, 1.f, -26.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CNpc_Wizard::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(-94.f, 1.f, -23.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// Boss
	//pGameObject = CSkeletonKing::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(0.f, 11.f, -30.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = COrb::Create(m_pGraphicDev, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(0.f, 3.5f, -30.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	return S_OK;
}

HRESULT CVillage::Ready_Layer_UI(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CUIPuzzleBack::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_PICTURE, 0);
	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_MAP, Engine::UILAYER::UI_MIDDLE, pGameObject);

	if (!Engine::SceneManager()->Get_VisitScene(m_eSceneTag))
	{
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
			Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_DOWN, pGameObject);
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
				Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
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
				Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_INVEN, Engine::UILAYER::UI_DOWN, pGameObject);
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

			Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
		}

		pGameObject = CUIplayerstat::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_STAT, Engine::UILAYER::UI_DOWN, pGameObject);

		// 조준점
		HCURSOR Cursor = nullptr;
		SetCursor(Cursor);
		pGameObject = CUIaimpoint::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_MOUSE, Engine::UILAYER::UI_DOWN, pGameObject);

		// 플레이어 hp bar
		pGameObject = CUIplayerhp::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_BasicGameobject(Engine::UILAYER::UI_DOWN, pGameObject);

		// 큰 지도
		pGameObject = CUIbigmap::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_MAP, Engine::UILAYER::UI_DOWN, pGameObject);

		// Esc 배경
		pGameObject = CUIEscBackground::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

		// Esc Resum 버튼
		pGameObject = CUIResumeButton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

		// Esc Option 버튼
		pGameObject = CUIOptionButton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

		// Esc Quition 버튼
		pGameObject = CUIQuitButton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_ESC, Engine::UILAYER::UI_DOWN, pGameObject);

		pGameObject = CUIShop::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SHOP, Engine::UILAYER::UI_DOWN, pGameObject);


		// Speech Bubble Test
		pGameObject = CUIspeech_OldMan::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

		pGameObject = CUIUseShop_Trander::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);


		pGameObject = CUIspeech_Bard::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

		pGameObject = CUISpeech_Alchemist::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

		pGameObject = CUIBark_Dog::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

		pGameObject = CUISpeech_Wizard::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_SPEECH, Engine::UILAYER::UI_DOWN, pGameObject);

		// 플레이어 레벨 업 UI
		pGameObject = CUILevelUp::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_LEVELUP, Engine::UILAYER::UI_DOWN, pGameObject);

		// 보스 hp 바 생성
		pGameObject = CUIbosshp::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_BOSSHP, Engine::UILAYER::UI_DOWN, pGameObject);

		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT);
		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_INVEN);
		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_STAT);
		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_MAP);
		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_ESC);
		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_SPEECH);	// Speech Bubble Test
		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_SHOP);
		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_LEVELUP);
		Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_BOSSHP);
	}
	
	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CVillage::Load_Data()
{
	CLayer* pLayer = m_mapLayer[LAYERTAG::GAMELOGIC];
	for (int i = 0; i < (UINT)OBJECTTAG::OBJECT_END; ++i)
	{
		// 일단 블록만
		if (OBJECTTAG::BLOCK != (OBJECTTAG)i || OBJECTTAG::TRAP != (OBJECTTAG)i || OBJECTTAG::IMMORTAL != (OBJECTTAG)i)
			continue;

		vector<CGameObject*>& refObjectList = pLayer->Get_ObjectList((OBJECTTAG)i);
		for_each(refObjectList.begin(), refObjectList.end(), [&](CGameObject* pObj) { EventManager()->DeleteObject(pObj); });
		refObjectList.clear();
	}
	//HANDLE hFile = CreateFile(L"../Bin/Data/Sewer.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(L"../Bin/Data/TempData.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE hFile = CreateFile(L"../Bin/Data/TerrainGiantTree12.dat", GENERIC_READ,	0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

			CGameObject* pGameObject = CCubeBlock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
			
			if (32 == byTextureNumber)
			{
				pGameObject = CWaterFall::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				dynamic_cast<CWaterFall*>(pGameObject)->Set_TextureNumber(0);
				pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1.f, fZ));
				pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
				//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
			else if (31 == byTextureNumber)
			{
				pGameObject = CWater::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				dynamic_cast<CWater*>(pGameObject)->Set_TextureNumber(byTextureNumber);
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
		else if (OBJECTTAG::IMMORTAL == eTag)
		{
			// value값 저장
			ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

			_float  fCX = 0.f, fCY = 0.f, fCZ = 0.f;
			ReadFile(hFile, &fCX, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fCY, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &fCZ, sizeof(_float), &dwByte, nullptr);

			ENVIRONMENTTAG eEnvTag;
			ReadFile(hFile, &eEnvTag, sizeof(ENVIRONMENTTAG), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CGameObject* pGameObject = nullptr;

			switch (eEnvTag)
			{
			case ENVIRONMENTTAG::TREE:
			{
				_uint iTreeNumber = 0;

				ReadFile(hFile, &iTreeNumber, sizeof(_uint), &dwByte, nullptr);

				pGameObject = CTree::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CTree*>(pGameObject)->Set_TreeNumber(iTreeNumber);
				break;
			}

			case ENVIRONMENTTAG::ROCK:
			{
				_uint iRockNumber = 0;

				ReadFile(hFile, &iRockNumber, sizeof(_uint), &dwByte, nullptr);

				pGameObject = CRock::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CRock*>(pGameObject)->Set_RockNumber(iRockNumber);
				break;
			}

			case ENVIRONMENTTAG::GRASS:
			{
				_uint iGrassNumber = 0;

				ReadFile(hFile, &iGrassNumber, sizeof(_uint), &dwByte, nullptr);

				pGameObject = CGrass::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CGrass*>(pGameObject)->Set_GrassNumber(iGrassNumber);
				break;
			}

			case ENVIRONMENTTAG::MUSHROOM:
			{
				_uint iMushroomNumber = 0;

				ReadFile(hFile, &iMushroomNumber, sizeof(_uint), &dwByte, nullptr);

				pGameObject = CMushroom::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CMushroom*>(pGameObject)->Set_MushroomNumber(iMushroomNumber);
				break;
			}

			case ENVIRONMENTTAG::PUMPKIN:
			{
				_uint iPumpkinNumber = 0;

				ReadFile(hFile, &iPumpkinNumber, sizeof(_uint), &dwByte, nullptr);

				pGameObject = CPumpkin::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CPumpkin*>(pGameObject)->Set_PumpkinNumber(iPumpkinNumber);
				break;
			}

			case ENVIRONMENTTAG::ETC:
			{
				_uint iSpriteNumber = 0;

				ReadFile(hFile, &iSpriteNumber, sizeof(_uint), &dwByte, nullptr);

				pGameObject = CImmortalSprite::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
				dynamic_cast<CImmortalSprite*>(pGameObject)->Set_SpriteNumber(iSpriteNumber);
				break;
			}

			}
			//pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(fX, fY, fZ);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->m_pTransform->Scale(_vec3(fCX, fCY, fCZ));
			pGameObject->m_pTransform->Translate(_vec3(fX, fY + 1, fZ));
			pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
			//EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
	}
	CloseHandle(hFile);
	return S_OK;
}

CVillage* CVillage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVillage* pInstance = new CVillage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CVillage::Free()
{
	//CPoolManager::DestroyInstance();
	__super::Free();
}
