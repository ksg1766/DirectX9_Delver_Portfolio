#pragma once

#include "Export_Function.h"
#include "TempParticle.h"

class CEffectSquare : public CTempParticle
{
private:
	explicit CEffectSquare(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectSquare();

public:
	void        Set_MonsterEffectColor(MONSTERTAG _MonsterTag);

public:
	HRESULT		Ready_Object(_vec3 vOriginPos, int numParticles, EFFECTCOLOR _Color);
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
	static CEffectSquare* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles, EFFECTCOLOR _Color);

private:
	virtual void Free();
};
