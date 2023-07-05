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

HRESULT		Create_SceneManager(LPDIRECT3DDEVICE9 pGraphicDev, CSceneManager** ppInstance)
{
	CSceneManager*		pSceneManager = CSceneManager::GetInstance();
	NULL_CHECK_RETURN(pSceneManager, E_FAIL);

	*ppInstance = pSceneManager;

	return S_OK;
}

//CComponent*			Get_Component(LAYERTAG eLayerTag,
//									OBJECTTAG eObjTag,
//									COMPONENTTAG eComponentTag,
//									COMPONENTID eID)
//{
//	return CSceneManager::GetInstance()->Get_Component(eLayerTag, eObjTag, eComponentTag, eID);
//}

//HRESULT			Set_Scene(CScene* pScene)
//{
//	return CSceneManager::GetInstance()->Set_Scene(pScene);
//}
//_int			Update_Scene(const _float& fTimeDelta)
//{
//	return CSceneManager::GetInstance()->Update_Scene(fTimeDelta);
//}
//void			LateUpdate_Scene()
//{
//	CSceneManager::GetInstance()->LateUpdate_Scene();
//}
//void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
//{
//	CSceneManager::GetInstance()->Render_Scene(pGraphicDev);
//}
//
//HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
//{
//	return CPrototypeManager::GetInstance()->Ready_Proto(pProtoTag, pComponent);
//}
//CComponent*		Clone_Proto(const _tchar* pProtoTag)
//{
//	return CPrototypeManager::GetInstance()->Clone_Proto(pProtoTag);
//}
//void		Add_RenderGroup(RENDERID eType, CGameObject* pGameObject)
//{
//	CRenderer::GetInstance()->Add_RenderGroup(eType, pGameObject);
//}
//void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
//{
//	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
//}
//void		Clear_RenderGroup()
//{
//	CRenderer::GetInstance()->Clear_RenderGroup();
//}


void			Release_Utility()
{
	CRenderer::GetInstance()->DestroyInstance();
	CEventManager::GetInstance()->DestroyInstance();
	CCollisionManager::GetInstance()->DestroyInstance();
	CPrototypeManager::GetInstance()->DestroyInstance();
	CUIManager::GetInstance()->DestroyInstance();
	CSceneManager::GetInstance()->DestroyInstance();
}