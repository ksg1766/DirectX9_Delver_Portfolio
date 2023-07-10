#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CSceneManager : public CBase
{
	DECLARE_SINGLETON(CSceneManager)

private:
	explicit CSceneManager();
	virtual ~CSceneManager();

public:
	_bool                   Get_GameStop()             { return m_bGameStop; }
	void                    Set_GameStop(_bool _bStop) { m_bGameStop = _bStop; }

	CLayer*					Get_Layer(LAYERTAG eLayerTag);
	vector<CGameObject*>&	Get_ObjectList(LAYERTAG eLayerTag, OBJECTTAG eObjTag)
							{ return m_pScene->Get_Layer(eLayerTag)->Get_ObjectList(eObjTag); }

public:
	CScene*			Get_Scene() { return m_pScene; }
	HRESULT			Set_Scene(CScene* pScene);

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene();
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*			m_pScene;
	_bool           m_bGameStop;

public:
	virtual void		Free();
};

END