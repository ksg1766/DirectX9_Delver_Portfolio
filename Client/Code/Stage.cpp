#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_Function.h"

#include "DungeonWarrior.h"
#include "DungeonSpider.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
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

	UIManager()->LateUpdate_UI();
}

void CStage::Render_Scene()
{
	// DEBUG ¿ë ·»´õ
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
	//Engine::EventManager()->CreateObject(pGameObject, _eLayerTag);
	
	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//Engine::EventManager()->CreateObject(pGameObject, _eLayerTag);


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
	//Engine::EventManager()->CreateObject(pGameObject, _eLayerTag);

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//Engine::EventManager()->CreateObject(pGameObject, _eLayerTag);
	dynamic_cast<CPlayer*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));

	// Monstser
	//pGameObject = CDungeonWarrior::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//dynamic_cast<CDungeonWarrior*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));

	//pGameObject = CDungeonSpider::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	//dynamic_cast<CDungeonSpider*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));

	//Boss
	pGameObject = CSkeletonKing::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);
	dynamic_cast<CSkeletonKing*>(pGameObject)->Set_Terrain(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList(OBJECTTAG::TERRAIN).front()));

	// TempCube
	/*pGameObject = CCubeBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);*/
	//Engine::EventManager()->CreateObject(pGameObject, _eLayerTag);
}

HRESULT CStage::Ready_Layer_UI(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CUIplayerhp::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);

	_float fX;
	for (_uint i = 0; i < 5; ++i)
	{
		pGameObject = CUIemptyslot::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].x = 520.f + (60.f * i);
		pGameObject->m_pTransform->m_vInfo[INFO_POS].y = WINCY - 30.f;
		dynamic_cast<CTempUI*>(pGameObject)->WorldMatrix(pGameObject->m_pTransform->m_vInfo[INFO_POS].x, pGameObject->m_pTransform->m_vInfo[INFO_POS].y, pGameObject->m_pTransform->m_vLocalScale.x, pGameObject->m_pTransform->m_vLocalScale.y);
		Engine::UIManager()->AddBasicGameobject_UI(Engine::UILAYER::UI_DOWN, pGameObject);
	}

	HCURSOR Cursor = nullptr;
	SetCursor(Cursor);
	pGameObject = CUIaimpoint::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::UIManager()->AddPopupGameobject_UI(Engine::UIPOPUPLAYER::POPUP_MOUSE, Engine::UILAYER::UI_DOWN, pGameObject);



	//pGameObject = CUItooltip::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//Engine::UIManager()->Add_UIGameobject(Engine::UILAYER::UI_DOWN, pGameObject);

	//pGameObject = CUIplayerstat::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//Engine::UIManager()->Add_UIGameobject(Engine::UILAYER::UI_DOWN, pGameObject);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}
