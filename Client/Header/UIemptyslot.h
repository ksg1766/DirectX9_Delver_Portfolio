#pragma once

#include "Export_Function.h"
#include "TempUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUIemptyslot : public CTempUI
{
private:
	explicit CUIemptyslot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIemptyslot();

public:
	HRESULT		Ready_Object();
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

	void        Set_FindSlot(_bool bFind)
	{
		m_bEquip = bFind;
		if (m_bEquip)
			m_fCurrentImage = 20;
	}

private:
	HRESULT		Add_Component(void);
	void	    Key_Input(void);

private:
	_bool     m_bEquip = false;

public:
	static CUIemptyslot*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
private:
	virtual void Free();

};

