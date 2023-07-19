#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectTwinkle : public CTempEffect
{
private:
	explicit CEffectTwinkle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectTwinkle();

public:
	void    Set_Distance(_float _distance) { m_fDistance = _distance; }

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	_vec3  m_vecOriginPos;
	_float m_fDistance;

public:
	static CEffectTwinkle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

