#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIbasicslot : public CTempUI
{
private:
	explicit CUIbasicslot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIbasicslot();

public:
	void        Set_FindSlot(_bool bFind)
	{
		m_bEquip = bFind;
		if (m_bEquip) {
			m_fCurrentImage = 20;
		}
	}

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

private:
	CTexture* m_pNumberTextureCom;
	_uint     m_fCurrentNumber;
	_bool     m_bEquip = false;

public:
	static CUIbasicslot*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

