#pragma once
#include "TempEffect.h"
class CEffectSwordParticles :
    public CTempEffect
{
private:
	explicit	CEffectSwordParticles(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CEffectSwordParticles();

public:
	HRESULT		Ready_Object()							override;
	_int		Update_Object(const _float& fTimeDelta) override;
	void		LateUpdate_Object(void)					override;
	void		Render_Object(void)						override;

public:

private:
	HRESULT		Add_Component(void);

private:
	_float		m_fCurScaleRate = 0.f;
	_float		m_fMaxScaleRate = 1.f;


public:
	static		CEffectSwordParticles* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

