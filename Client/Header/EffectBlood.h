#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectBlood : public CTempEffect
{
private:
	explicit CEffectBlood(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectBlood();

public:
	void Set_BloodColor(_uint Number) { m_iBloodColor = Number; }
	// 0¹ø »¡°­ / 1¹ø ÃÊ·Ï / 2¹ø ³ë¶û

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	_uint m_iBloodColor = 0.f;

public:
	static CEffectBlood*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

