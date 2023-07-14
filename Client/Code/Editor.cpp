#include "..\Header\Editor.h"
#include "ImGuiManager.h"
#include "CameraManager.h"

CEditor::CEditor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CEditor::~CEditor()
{
}

HRESULT CEditor::Ready_Scene()
{
	m_eSceneTag = SCENETAG::EDITOR;

	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(LAYERTAG::GAMELOGIC), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(LAYERTAG::UI), E_FAIL);

	CImGuiManager::GetInstance()->SetUp_ImGui();

	//Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BLOCK).reserve(50 * 50 * 50);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

_int CEditor::Update_Scene(const _float& fTimeDelta)
{
	__super::Update_Scene(fTimeDelta);

	CCameraManager::GetInstance()->Update_Camera();
	CImGuiManager::GetInstance()->Update_ImGui(fTimeDelta);

	return 0;
}

void CEditor::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();

	CCameraManager::GetInstance()->LateUpdate_Camera();
	CImGuiManager::GetInstance()->LateUpdate_ImGui();
}

void CEditor::Render_Scene()
{
	// DEBUG ¿ë ·»´õ
	CImGuiManager::GetInstance()->Render_ImGui(m_pGraphicDev);
}

HRESULT CEditor::Ready_Prototype()
{
	return S_OK;
}

HRESULT CEditor::Ready_Layer_Environment(LAYERTAG _eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	// FlyingCamera
	pGameObject = CFlyingCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// OrthoCamera
	pGameObject = COrthoCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

HRESULT CEditor::Ready_Layer_GameLogic(LAYERTAG _eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ _eLayerTag, pLayer });

	Engine::CGameObject* pGameObject = nullptr;

	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	pGameObject = CCubeBlock::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->m_pTransform->Translate(_vec3(1.f, 1.f, 1.f));
	pLayer->Add_GameObject(pGameObject->Get_ObjectTag(), pGameObject);

	return S_OK;
}

HRESULT CEditor::Ready_Layer_UI(LAYERTAG _eLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}

CEditor* CEditor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEditor* pInstance = new CEditor(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Editor Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEditor::Free()
{
	__super::Free();
}
