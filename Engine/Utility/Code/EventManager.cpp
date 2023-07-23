#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CEventManager)

CEventManager::CEventManager()
{
}


CEventManager::~CEventManager()
{
}

void CEventManager::Update()
{
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		// 여기에서 삭제 해주고, GameScene에서는 벡터 원소만 날려 줌.
		/*if (OBJ_TYPE::MONSTER == m_vecDead[i]->GetType())
		{
			CManagers::instance().Pool()->ReturnPool(m_vecDead[i]);
		}*/
		Safe_Release(m_vecDead[i]);		
	}
	m_vecDead.clear();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Execute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

void CEventManager::CreateObject(CGameObject * _pObj, LAYERTAG _eLayer)
{
	tagEvent evn = {};
	evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;
	evn.wParam = (DWORD_PTR)_eLayer;

	AddEvent(evn);
}

void CEventManager::DeleteObject(CGameObject * _pObj)
{
	tagEvent evn = {};
	evn.eEvent = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;

	AddEvent(evn);
}

void CEventManager::Execute(const tagEvent & _eve)
{
	switch (_eve.eEvent)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		CGameObject* pNewObject = (CGameObject*)_eve.lParam;
		LAYERTAG eType = (LAYERTAG)_eve.wParam;

		SceneManager()->Get_Layer(eType)->Add_GameObject(pNewObject->Get_ObjectTag(), pNewObject);
	}
	break;

	case EVENT_TYPE::DELETE_OBJECT:
	{
		CGameObject*	pDeadObject = (CGameObject*)_eve.lParam;

		const OBJECTTAG& eObjectTag = pDeadObject->Get_ObjectTag();
		if (OBJECTTAG::MONSTER != eObjectTag && OBJECTTAG::EFFECT != eObjectTag)
		{
			pDeadObject->Set_Dead(true);
			m_vecDead.push_back(pDeadObject);
		}
	}
	break;
	case EVENT_TYPE::SCENE_CHANGE:
		//
		break;
	}
}

void CEventManager::Free()
{
}
