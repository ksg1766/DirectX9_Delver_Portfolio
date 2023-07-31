#pragma once

#include "Export_Function.h"
#include "TempParticle.h"

class CEffectWater : public CTempParticle
{
private:
	explicit CEffectWater(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectWater();

public:
	HRESULT		Ready_Object(_vec3 vOriginPos, int numParticles, float fSize, EFFECTCOLOR _Color);
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

private:
	void        Initial_Particle(ParticleAttribute* _attribute);
	void        Read_Path(EFFECTCOLOR _color);

private:
	_float	    m_fTime = 0.f;
	_float      m_fSpeed = 0.f;

public:
	static CEffectWater* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles, float fSize, EFFECTCOLOR _Color);

private:
	virtual void Free();
};
