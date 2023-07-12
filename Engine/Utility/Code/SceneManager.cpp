#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager() : m_pScene(nullptr), m_bGameStop(false)
{
}

CSceneManager::~CSceneManager()
{
	Free();
}

CLayer * CSceneManager::Get_Layer(LAYERTAG eLayerTag)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Layer(eLayerTag);
}

HRESULT CSceneManager::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);

	Renderer()->Clear_RenderGroup();
	
	m_pScene = pScene;

	return S_OK;
}

_int CSceneManager::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	return m_pScene->Update_Scene(fTimeDelta);
}

void CSceneManager::LateUpdate_Scene()
{
	if (nullptr == m_pScene)
		return;

	m_pScene->LateUpdate_Scene();
}

void CSceneManager::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
#ifdef _DEBUG
	//if(SCENETAG::STAGE == m_pScene->Get_SceneTag())	Octree()->Render_Octree(pGraphicDev);
#endif

	Renderer()->Render_GameObject(pGraphicDev);

	if (m_pScene)
		m_pScene->Render_Scene();	

}

HRESULT CSceneManager::Load_Data()
{
	if (m_pScene)
		m_pScene->Load_Data();
	return E_NOTIMPL;
}

void CSceneManager::Free()
{
	Safe_Release(m_pScene);
}
