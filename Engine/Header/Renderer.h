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

	void        Set_FogUse(_bool _bUse) { m_bFogUse = _bUse; }
	void        Set_FogColor(_int _A, _int _R, _int _G, _int _B) {
		m_iFogColor[0] = _A;
		m_iFogColor[1] = _R;
		m_iFogColor[2] = _G;
		m_iFogColor[3] = _B;
	}
	void        Set_FogDistance(_float _fnear, _float _ffar) 
	{ 
		m_fFogNear = _fnear; 
		m_fFogFar  = _ffar;
	}

public:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Nonalpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void        Remder_Effect(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);

private:
	list<CGameObject*>			m_RenderGroup[RENDER_END];
	_bool  m_bFogUse;
	_int   m_iFogColor[4];
	_float m_fFogNear;
	_float m_fFogFar;

private:
	virtual void		Free();
};

END
