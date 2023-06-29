#pragma once

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CLayer*					Get_Layer(LAYERTAG eLayerTag);
	vector<CGameObject*>&	Get_ObjectList(LAYERTAG eLayerTag, OBJECTTAG eObjTag) 
							{ return m_mapLayer[eLayerTag]->Get_ObjectList(eObjTag);}

public:
	virtual HRESULT		Ready_Scene();
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		LateUpdate_Scene();
	virtual void		Render_Scene()PURE;

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<LAYERTAG, CLayer*>			m_mapLayer;

public:
	virtual void	Free();
};

END