#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectWaterBubble : public CTempEffect
{
private:
	explicit CEffectWaterBubble(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectWaterBubble();

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:

public:
	static CEffectWaterBubble* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

