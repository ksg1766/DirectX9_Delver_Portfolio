#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectWaterdrop : public CTempEffect
{
private:
	explicit CEffectWaterdrop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectWaterdrop();

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:

public:
	static CEffectWaterdrop* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

