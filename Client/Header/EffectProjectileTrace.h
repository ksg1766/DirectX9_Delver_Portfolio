#pragma once

#include "Export_Function.h"
#include "TempEffect.h"

class CEffectProjectileTrace : public CTempEffect
{
private:
	explicit CEffectProjectileTrace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectProjectileTrace();

public:
	HRESULT Ready_Object() override;
	_int    Update_Object(const _float& fTimeDelta) override;
	void    LateUpdate_Object(void) override;
	void    Render_Object(void) override;

private:
	HRESULT	Add_Component(void);

private:
	_vec3		 m_vecDir;

	_float       m_fCreatTime  = 0.f;
	_float       m_fCreatCount = 200.f;
	_float       m_fSpeed = 0.f;

public:
	static CEffectProjectileTrace*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Set_vDir(_vec3 _vInfo);
private:
	virtual void Free() override;

};

