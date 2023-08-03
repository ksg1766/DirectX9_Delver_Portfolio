#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUICredit : public CTempUI
{
private:
	explicit CUICredit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUICredit();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

public:
	static CUICredit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

