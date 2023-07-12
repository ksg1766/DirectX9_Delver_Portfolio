#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"

#include "DungeonWarrior.h"
#include "DungeonSpider.h"
#include "Bat.h"
#include "Wizard.h"
#include "Alien.h"

#include "Boss_BatSwarm.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	m_eSceneTag = SCENETAG::STAGE;

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);

	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

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
}

void CStage::Render_Scene()
{
	// DEBUG 용 렌더
}

void CStage::Free()
{
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

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, 
											&_vec3(0.f, 10.f, -10.f),
											&_vec3(0.f, 0.f, 1.f),
											&_vec3(0.f, 1.f, 0.f),
											D3DXToRadian(60.f), 
											(_float)WINCX / WINCY,
											0.1f, 
											1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	
	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);


	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	Engine::CGameObject*		pGameObject = nullptr;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CPlayer*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));

	// Monstser
	pGameObject = CDungeonWarrior::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CDungeonWarrior*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));
	
	pGameObject = CDungeonSpider::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CDungeonSpider*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));
	
	pGameObject = CBat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CBat*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));
	
	pGameObject = CWizard::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CWizard*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));
	
	pGameObject = CAlien::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CAlien*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));

	//// Boss
	//pGameObject = CSkeletonKing::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//dynamic_cast<CSkeletonKing*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));

	// TempCube0
	pGameObject = CCubeBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(4.f, 2.f, 4.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	//// TempCube1
	//pGameObject = CCubeBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(8.f, 2.f, 4.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	//// TempCube2
	//pGameObject = CCubeBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(12.f, 2.f, 4.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	//// TempCube3
	//pGameObject = CCubeBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(8.f, 2.f, 8.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	//// TempCube4
	//pGameObject = CCubeBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(8.f, 2.f, 12.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//
	//// TempCube5
	//pGameObject = CCubeBlock::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->m_pTransform->Translate(_vec3(8.f, 6.f, 8.f));
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);


	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// 플레이어 hp bar
	pGameObject = CUIplayerhp::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);

	// 기본 인벤토리 5칸
	for (_uint i = 0; i < 5; ++i) {
		pGameObject = CUIbasicslot::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 520.f + (60.f * i);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].y = WINCY - 30.f;
		dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
		dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_SLOTBASIC, i);
		dynamic_cast<CTempUI*>(pGameObject)->Set_Setup(true);
		Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);
	}

	// 조준점
	HCURSOR Cursor = nullptr;
	SetCursor(Cursor);
	pGameObject = CUIaimpoint::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_MOUSE, Engine::UILAYER::UI_DOWN, pGameObject);

	_uint Index = 0;
	// 장비슬롯
	for (_uint iy = 0; iy < 3; ++iy) {
		for (_uint ix = 0; ix < 2; ++ix) {

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
	for (_uint iy = 0; iy < 3; ++iy) {
		for (_uint ix = 0; ix < 6; ++ix) {

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
	for (_uint iy = 0; iy < 2; ++iy) {
		pGameObject = CUIbutton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 850.f;
		pGameObject->m_pTransform->m_vInfo[INFO_POS].y = 595.f - (40 * iy);
		dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
		dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_INVENBUTTON, iy);
		if (iy == 0) {
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIImage(1);
		}
		else {
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIImage(3);
		}

		Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT, Engine::UILAYER::UI_DOWN, pGameObject);
	}

	pGameObject = CUIplayerstat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_STAT, Engine::UILAYER::UI_DOWN, pGameObject);

	//for (int i = 0; i < 23; ++i)
	//{
	//	pGameObject = CUIitem::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	Engine::UIManager()->AddItemGameobject_UI(pGameObject);
	//}

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

	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_EQUIPMENT);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_INVEN);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_STAT);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_MAP);
	Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_ESC);

	//pGameObject = CUItooltip::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}
