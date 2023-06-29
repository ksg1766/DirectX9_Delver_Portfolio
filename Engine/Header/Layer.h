#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	explicit CLayer(LAYERTAG _eLayerTag);
	virtual ~CLayer();

public:
	vector<CGameObject*>&	Get_ObjectList(OBJECTTAG eObjTag) { return m_mapObject[eObjTag]; }

	map<OBJECTTAG, vector<CGameObject*>>&	Get_Map() { return m_mapObject; }

public:
	HRESULT			Add_GameObject(OBJECTTAG eObjTag, CGameObject* pGameObject);

	HRESULT			Ready_Layer();
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer();

private:
	map<OBJECTTAG, vector<CGameObject*>>	m_mapObject;
	LAYERTAG		m_eLayerTag;

public:
	static		CLayer*		Create(LAYERTAG	 _eLayerTag);
	virtual		void		Free();
};

END