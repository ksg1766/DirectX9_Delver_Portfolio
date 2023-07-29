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
	CLayer*					Get_Layer(LAYERTAG eLayerTag);
	vector<CGameObject*>&	Get_ObjectList(LAYERTAG eLayerTag, OBJECTTAG eObjTag)
	{ return m_pCurrentScene->Get_Layer(eLayerTag)->Get_ObjectList(eObjTag); }

public:
	CScene*			Get_Scene() { return m_pCurrentScene; }
	HRESULT			Set_Scene(CScene* pScene);
	HRESULT			Change_Scene(CScene* pScene);

	_bool           Get_VisitScene(SCENETAG _SceneTag) { return  m_bStageVisit[(unsigned long long)_SceneTag]; }

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene();
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

	HRESULT			Load_Data();

public:
	void			SlowMode(_uint _bySlowDuration, _ubyte _bySlowConstant);

	_bool           Get_GameStop()				{ return m_bGameStop; }
	_byte			Get_SlowConstant()			{ return m_bySlowRate; }
	_bool			Is_TickPassed()				{ return m_bPassTick; }

	void            Set_GameStop(_bool _bStop)	{ m_bGameStop = _bStop; }
	void            Set_SlowConstant(_ubyte _bySlowRate) { m_bySlowRate = _bySlowRate; }

private:
	CScene*         m_pBeforScene;
	CScene*			m_pCurrentScene;
	CScene*         m_pNextScene;
	
	_bool           m_bGameStop;
	_bool           m_bSceneChange;

	_bool			m_bPassTick	= false;
	_ubyte			m_bySlowRate = 0;
	_float			m_fSlowDuration = 0;
	_uint			m_iUpdateCount = 0;

	_bool           m_bStageVisit[(unsigned long long)SCENETAG::SCENE_END];

public:
	virtual void		Free();
};

END