#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

public:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	void		Add_RenderGroup(RENDERID eType, CGameObject* pGameObject);
	void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Clear_RenderGroup();

public:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Nonalpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];

private:
	virtual void		Free();
};

END
