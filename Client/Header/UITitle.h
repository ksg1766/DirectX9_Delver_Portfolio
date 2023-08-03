#pragma once

#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUITitle : public CTempUI
{
private:
	explicit CUITitle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUITitle();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);

private:
	_float		m_fAlpha;
	_bool		m_bFirst = true;
	_bool		m_bHide = false;

public:
	static CUITitle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

