#pragma once

#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIPuzzle : public CTempUI
{
private:
	explicit CUIPuzzle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIPuzzle();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

public:
	static CUIPuzzle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

