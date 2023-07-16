#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectDamage : public CTempEffect
{
private:
	explicit CEffectDamage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectDamage();

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:

public:
	static CEffectDamage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

