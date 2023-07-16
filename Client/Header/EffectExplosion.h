#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectExplosion : public CTempEffect
{
private:
	explicit CEffectExplosion(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectExplosion();

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:

public:
	static CEffectExplosion*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

