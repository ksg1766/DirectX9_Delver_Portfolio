#include "..\..\Header\Scene.h"
#include "Export_Function.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CLayer * CScene::Get_Layer(LAYERTAG eLayerTag)
{
	return m_mapLayer[eLayerTag];
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	_int		iResult = 0;

	for (auto& iter : m_mapLayer)
	{
		iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	if (SCENETAG::VILLAGE == m_eSceneTag || SCENETAG::STAGE == m_eSceneTag || SCENETAG::BOSSSTAGE == m_eSceneTag)
	{
		vector<CGameObject*>& vecCamera = m_mapLayer[LAYERTAG::ENVIRONMENT]->Get_ObjectList(OBJECTTAG::CAMERA);
		if (!vecCamera.empty())
			iResult = vecCamera.front()->Update_Object(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CScene::LateUpdate_Scene()
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer();
}

void CScene::Free()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
