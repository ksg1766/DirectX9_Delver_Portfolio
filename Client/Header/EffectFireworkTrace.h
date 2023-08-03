#pragma once

#include "Export_Function.h"
#include "TempParticle.h"

class CEffectFireworkTrace : public CTempParticle
{
private:
	explicit CEffectFireworkTrace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectFireworkTrace();

public:
	HRESULT		Ready_Object(_vec3 vOriginPos, int numParticles);
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	void        Initial_Particle(ParticleAttribute* _attribute);

private:
	_float	    m_fTime = 0.f;
	_float      m_fSpeed = 0.f;
	_float      m_fDieTime = 0.f;
	_float      m_fMoveTime = 0.f;

public:
	static CEffectFireworkTrace* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles);

private:
	virtual void Free();
};
