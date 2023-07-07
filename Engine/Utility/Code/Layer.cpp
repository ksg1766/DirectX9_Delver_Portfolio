#include "..\..\Header\Layer.h"
#include "Export_Utility.h"

CLayer::CLayer():m_eLayerTag(LAYERTAG::LAYER_END)
{
}

CLayer::CLayer(LAYERTAG _eLayerTag) : m_eLayerTag(_eLayerTag)
{

}

CLayer::~CLayer()
{
}

HRESULT CLayer::Add_GameObject(OBJECTTAG eObjTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject[eObjTag].push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int		iResult = 0;

	for (auto& iter : m_mapObject)
	{
		for(auto& _iter = iter.second.begin(); _iter != iter.second.end();)
		{
			if (!(*_iter)->IsDead())
			{
				iResult = (*_iter)->Update_Object(fTimeDelta);
				++_iter;

				if (iResult & 0x80000000)
					return iResult;
			}
			else
				_iter = iter.second.erase(_iter);
		}
	}
	
	return iResult;
}

void CLayer::LateUpdate_Layer()
{
	for (auto& iter : m_mapObject)
		for (auto& _iter : iter.second)
			_iter->LateUpdate_Object();
}

CLayer * CLayer::Create(LAYERTAG _eLayerTag)
{
	CLayer *	pInstance = new CLayer(_eLayerTag);

	if (FAILED(pInstance->Ready_Layer()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Layer Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteVector());
	m_mapObject.clear();
}
