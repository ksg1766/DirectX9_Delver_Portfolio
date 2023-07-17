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

private:
	HRESULT		Add_Component(void);

private:
	_float m_fDownSpeed;
	_vec3  m_vecVelocity;


public:
	static CEffectBrokenbox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

