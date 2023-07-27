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

void CEventManager::SceneChange(CScene* _pScene, SCENETAG _eSceneTag)
{
	tagEvent evn = {};
	evn.eEvent = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)_pScene;
	evn.wParam = (DWORD_PTR)_eSceneTag;

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
		//if (OBJECTTAG::MONSTER != eObjectTag && OBJECTTAG::EFFECT != eObjectTag)
		if (OBJECTTAG::MONSTER != eObjectTag)
		{
			pDeadObject->Set_Dead(true);
			m_vecDead.push_back(pDeadObject);
		}
	}
	break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		CScene*		pNewScene = (CScene*)_eve.lParam;
		SCENETAG	eSceneTag = (SCENETAG)_eve.wParam;

		// 여기서 씬매니저 씬 전환 불 값 바꿔주고
		// Load_Data() 하면 블록이 전부 DeleteObject로 넘어가는데 원래 씬의 벡터는 다음 프레임에 IsDead가 True인 애들을 삭제할 것임.
		// (그리고 아마 그 다음 프레임에 객체들도 삭제 될 것.)
		// 벡터 초기화 이후에 씬이 넘어가야하니까 
	}
	break;
	}
}

void CEventManager::Free()
{
}
