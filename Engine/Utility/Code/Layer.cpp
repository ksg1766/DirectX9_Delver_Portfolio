#include "..\..\Header\Layer.h"
#include "Export_Utility.h"
#include "Trap.h"

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
	_int     iResult = 0;
	SCENETAG eSceneTag = SceneManager()->Get_Scene()->Get_SceneTag();
	// Stage
	if (SCENETAG::VILLAGE == eSceneTag || SCENETAG::STAGE == eSceneTag || SCENETAG::BOSSSTAGE == eSceneTag)
	{
		for (_uint i = 0; i < (_uint)OBJECTTAG::OBJECT_END; ++i)
		{
			if (i == (_uint)OBJECTTAG::BLOCK || i == (_uint)OBJECTTAG::BACKGROUND || i == (_uint)OBJECTTAG::CAMERA || i == (_uint)OBJECTTAG::IMMORTAL)
				continue;

			for (auto& _iter = m_mapObject[(OBJECTTAG)i].begin(); _iter != m_mapObject[(OBJECTTAG)i].end();)
			{
				if (!(*_iter)->IsDead() && *_iter != nullptr)
				{
					if ((OBJECTTAG)i == OBJECTTAG::TRAP)
					{
						if (TRAPTAG::STRIKEDOWN != static_cast<CTrap*>(*_iter)->Get_TrapTag())
						{
							++_iter;
							continue;
						}
					}

					iResult = (*_iter)->Update_Object(fTimeDelta);
					++_iter;

					if (iResult & 0x80000000)
						return iResult;
				}
				else
					_iter = m_mapObject[(OBJECTTAG)i].erase(_iter);
			}
		}
	}

	// Editor
	else if (SCENETAG::EDITOR == eSceneTag)
	{
		for (auto& iter : m_mapObject)
		{
			for (auto& _iter = iter.second.begin(); _iter != iter.second.end();)
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
	}
	return iResult;
}

void CLayer::LateUpdate_Layer()
{
	for (auto& iter : m_mapObject)
	{	
		for (auto& _iter : iter.second)
		{
			if ((_iter)->Get_ObjectTag() != OBJECTTAG::BACKGROUND)
			{
				_iter->LateUpdate_Object();
				if ((_iter)->Get_ObjectTag() == OBJECTTAG::TRAP)
				{
					if (TRAPTAG::STRIKEDOWN != static_cast<CTrap*>(_iter)->Get_TrapTag())
						continue;
				}
			}
		}
	}
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
	//for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteVector());
	//m_mapObject.clear();

	for (auto& Mapiter : m_mapObject)
	{
		for (auto iter : Mapiter.second)
		{
			if (iter != nullptr &&
				SCENETAG::BOSSSTAGE != SceneManager()->Get_Scene()->Get_SceneTag() && // 임시 : 중간에 껐을 시 메모리 해제 안될 수 있음
				iter->Get_ObjectTag() != OBJECTTAG::CAMERA &&
				iter->Get_ObjectTag() != OBJECTTAG::PLAYER &&
				iter->Get_ObjectTag() != OBJECTTAG::ITEM &&
				iter->Get_ObjectTag() != OBJECTTAG::RAY)
			{
				Safe_Release(iter);
				Mapiter.second.clear();
			}		
		}
	}
}
