#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIOrbClearLight : public CTempUI
{
private:
	explicit CUIOrbClearLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIOrbClearLight();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

private:
	float m_fFrame; 
	float m_fFrist; 
	float m_fFinal; 
	float m_fFrameSpeed;
	_bool m_bAnimation = true;

public:
	static CUIOrbClearLight*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

