#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectPastTrace : public CTempEffect
{
private:
	explicit CEffectPastTrace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectPastTrace();

public:
	void    Set_EffectDir(_vec3 _Dir) { m_vecDir = _Dir; }

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT		Add_Component(void);

private:
	_vec3        m_pPlayerInfo;
	_vec3		 m_vecDir;

	_float       m_fCreatTime = 0.f;
	_float       m_fSpeed  = 0.f;
	_float       m_fZangle = 0.f;

	_float       m_fXScale = 0.f;

public:
	static CEffectPastTrace*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;

};

