#include "Export_Utility.h"
inline CSceneManager * SceneManager()
{
	return CSceneManager::GetInstance();
}

inline CPrototypeManager * PrototypeManager()
{
	return CPrototypeManager::GetInstance();
}

inline CCollisionManager * CollisionManager()
{
	return CCollisionManager::GetInstance();
}

inline CEventManager * EventManager()
{
	return CEventManager::GetInstance();
}

inline CRenderer * Renderer()
{
	return CRenderer::GetInstance();
}

inline CUIManager* UIManager()
{
	return CUIManager::GetInstance();
}

inline COctree* Octree()
{
	return COctree::GetInstance();
}

HRESULT		Create_SceneManager(LPDIRECT3DDEVICE9 pGraphicDev, CSceneManager** ppInstance)
{
	CSceneManager*		pSceneManager = CSceneManager::GetInstance();
	NULL_CHECK_RETURN(pSceneManager, E_FAIL);

	*ppInstance = pSceneManager;

	return S_OK;
}

void			Release_Utility()
{
	CRenderer::GetInstance()->DestroyInstance();
	CEventManager::GetInstance()->DestroyInstance();
	CCollisionManager::GetInstance()->DestroyInstance();
	CPrototypeManager::GetInstance()->DestroyInstance();
	CUIManager::GetInstance()->DestroyInstance();
	CSceneManager::GetInstance()->DestroyInstance();
	COctree::GetInstance()->DestroyInstance();
}