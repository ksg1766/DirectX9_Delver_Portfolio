#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectFirefly : public CTempEffect
{
private:
	explicit CEffectFirefly(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectFirefly();

public:

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	_float m_bMoveSpeed = 0.f;
	_vec3  m_vecMoveDir;

public:
	static CEffectFirefly*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

