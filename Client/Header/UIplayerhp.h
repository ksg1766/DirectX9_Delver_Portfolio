#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIplayerhp : public CTempUI
{
private:
	explicit CUIplayerhp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIplayerhp();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);
	void        Update_NumverUI(void);

private:
	CTexture*   m_pNumberTextureCom;
	_float      m_fCurrentHp;
	_float      m_fMaxHp;

	_int        m_iCurrentOneNum;
	_int        m_iCurrentTwoNum;
	_int        m_iMaxOneNum;
	_int        m_iMaxTwoNum;

	_float      m_fPosition;
	_float      m_flength;

	_bool       m_bRender = true;
	_float	    m_fTime = 0.f;
	_uint       iIndex  = 3;
		
public:
	static CUIplayerhp*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

