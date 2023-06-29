#include "stdafx.h"
#include "..\Header\Logo.h"

#include "Export_Function.h"
#include "Stage.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(LAYERTAG::ENVIRONMENT), E_FAIL);
	
	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	return S_OK;
}

Engine::_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit =__super::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		CScene*		pScene = CStage::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, -1);

		FAILED_CHECK_RETURN(Engine::SceneManager()->Set_Scene(pScene), E_FAIL);
	}

	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	// DEBUG ¿ë ·»´õ
}

void CLogo::Free()
{
	Safe_Release(m_pLoading);

	__super::Free();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CLogo::Ready_Prototype()
{
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::PrototypeManager()->Ready_Proto(L"Proto_Texture_Logo", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/IU.jpg")), E_FAIL);

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Environment(LAYERTAG _eLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(_eLayerTag);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// BackGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	Engine::EventManager()->CreateObject(pGameObject, _eLayerTag);

	
	m_mapLayer.insert({ _eLayerTag, pLayer });

	return S_OK;
}
