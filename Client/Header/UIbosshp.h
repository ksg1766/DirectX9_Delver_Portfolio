#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIbosshp : public CTempUI
{
private:
	explicit CUIbosshp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIbosshp();

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

	_float      m_fPosition;
	_float      m_flength;
		
public:
	static CUIbosshp* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

