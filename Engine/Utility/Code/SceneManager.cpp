#include "Export_Utility.h"
#include "../Client/Header/Player.h"

IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager() : m_pBeforScene(nullptr), m_pCurrentScene(nullptr), m_pNextScene(nullptr), m_bGameStop(false), m_bSceneChange(false)
{
	for (_uint i = 0; (SCENETAG)i < SCENETAG::SCENE_END; ++i)
	{
		m_bStageVisit[i] = false;
	}
}

CSceneManager::~CSceneManager()
{
	Free();
}

CLayer * CSceneManager::Get_Layer(LAYERTAG eLayerTag)
{
	if (nullptr == m_pCurrentScene)
		return nullptr;

	return m_pCurrentScene->Get_Layer(eLayerTag);
}

HRESULT CSceneManager::Set_Scene(CScene* pScene)
{
	Safe_Release(m_pCurrentScene);
	Renderer()->Clear_RenderGroup();

	m_pCurrentScene = pScene;
	m_bStageVisit[(unsigned long long)pScene->Get_SceneTag()] = true;

	return S_OK;
}

HRESULT CSceneManager::Change_Scene(CScene* pScene)
{
	m_bSceneChange = true;

	m_pBeforScene = m_pCurrentScene;
	m_pNextScene = pScene;

	// 이전 씬을 방문 여부를 기록
	m_bStageVisit[(_uint)pScene->Get_SceneTag() - 1] = true;

	return S_OK;
}

_int CSceneManager::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	if (m_bSceneChange)
	{
		Octree()->Ready_Octree();
		m_bSceneChange = false;
	}
	
#pragma region SlowMode

	if (m_fSlowDuration > 0.f)
	{
		m_bPassTick = false;
		m_fSlowDuration -= fTimeDelta;
		if (m_bySlowRate - 1 > m_iUpdateCount++)
		{
			m_bPassTick = true;
			return _int();
		}

		m_iUpdateCount = 0;
	}
	else
	{
		m_bPassTick = false;
		m_iUpdateCount = 0;
	}

#pragma endregion SlowMode

	Octree()->Update_Octree();
	m_pCurrentScene->Update_Scene(fTimeDelta);

	return _int();
}

void CSceneManager::LateUpdate_Scene()
{
	if (nullptr == m_pCurrentScene)
		return;

	if(!m_bPassTick)
		m_pCurrentScene->LateUpdate_Scene();
}

void CSceneManager::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
#ifdef _DEBUG
	//if(SCENETAG::STAGE == m_pScene->Get_SceneTag())	Octree()->Render_Octree(pGraphicDev);
#endif

	if (SCENETAG::VILLAGE == m_pCurrentScene->Get_SceneTag() || SCENETAG::STAGE == m_pCurrentScene->Get_SceneTag() || SCENETAG::BOSSSTAGE == m_pCurrentScene->Get_SceneTag())
		Octree()->Render_Octree(pGraphicDev);

	Renderer()->Render_GameObject(pGraphicDev);

	if (m_pCurrentScene)
		m_pCurrentScene->Render_Scene();

	if (m_bSceneChange)
	{
		CLayer* pEnvironmentObj = Get_Layer(LAYERTAG::ENVIRONMENT);
		CLayer* pGameLogic      = Get_Layer(LAYERTAG::GAMELOGIC);

		if (pEnvironmentObj != nullptr && pGameLogic != nullptr)
		{
			CGameObject* m_pPlayer         = pGameLogic->Get_ObjectList(OBJECTTAG::PLAYER).front();
			CGameObject* m_pCamera         = pEnvironmentObj->Get_ObjectList(OBJECTTAG::CAMERA).front();
			CGameObject* m_pPlayerRay      = pGameLogic->Get_ObjectList(OBJECTTAG::RAY).front();
			vector<CGameObject*> m_vecItem = pGameLogic->Get_ObjectList(OBJECTTAG::ITEM);

			Safe_Release(m_pBeforScene);
			Renderer()->Clear_RenderGroup();
			m_pCurrentScene = m_pNextScene;

			switch (m_pCurrentScene->Get_SceneTag())
			{
			case SCENETAG::VILLAGE:
				m_pPlayer->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.f, 4.f, -70.f);
				break;
			case SCENETAG::STAGE:
				m_pPlayer->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.f, 11.f, 0.f);  // 시작 위치 지정해주세요
				break;
			case SCENETAG::BOSSSTAGE:
				m_pPlayer->m_pTransform->m_vInfo[INFO_POS] = _vec3(100.f, 20.f, 0.f); // 시작 위치 지정해주세요
				break;
			}

			m_pCurrentScene->Set_MainPlayer(dynamic_cast<CPlayer*>(m_pPlayer));

			m_pCamera->m_pTransform->Copy_RUL_AddPos(m_pPlayer->m_pTransform->m_vInfo);
			Engine::EventManager()->CreateObject(m_pCamera,    LAYERTAG::ENVIRONMENT);
			Engine::EventManager()->CreateObject(m_pPlayer,    LAYERTAG::GAMELOGIC);
			Engine::EventManager()->CreateObject(m_pPlayerRay, LAYERTAG::GAMELOGIC);

			for (auto& iter : m_vecItem)
			{
				Engine::EventManager()->CreateObject(iter, LAYERTAG::GAMELOGIC);
			}

			Octree()->DestroyInstance();
			Load_Data();
		}
	}
}

HRESULT CSceneManager::Load_Data()
{
	if (m_pCurrentScene)
		m_pCurrentScene->Load_Data();

	return S_OK;
}

void CSceneManager::SlowMode(_uint _bySlowDuration, _ubyte _bySlowRate)
{
	m_fSlowDuration = _bySlowDuration;
	m_bySlowRate = _bySlowRate;
}

void CSceneManager::Free()
{
	Safe_Release(m_pCurrentScene);
}
