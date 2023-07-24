#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectBrokenbox : public CTempEffect
{
private:
	explicit CEffectBrokenbox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectBrokenbox();

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

public:
	void	Set_EffectType(_uint _iType) 
	{
		m_iType = _iType; 
		m_bBoxSet = true;
	}

private:
	HRESULT		Add_Component(void);

private:
	_float m_fDownSpeed;
	_uint  m_iType;
	_bool  m_bBoxSet = false;
	
public:
	static CEffectBrokenbox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

