#pragma once

#include "Export_Function.h"
#include "TempParticle.h"

class CEffectSquare : public CTempParticle
{
private:
	explicit CEffectSquare(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffectSquare();

public:
	HRESULT		Ready_Object(_vec3 vOriginPos, int numParticles, ParticleBoundingBox* boundbox, const _tchar* pPath);
	_int		Update_Object(const _float& fTimeDelta);
	void		LateUpdate_Object(void);
	void		Render_Object();

public:
	virtual void	OnCollisionEnter(CCollider* _pOther);
	virtual void	OnCollisionStay(CCollider* _pOther);
	virtual void	OnCollisionExit(CCollider* _pOther);

private:
	void        Initial_Particle(ParticleAttribute* _attribute);

private:
	_float	    m_fTime = 0.f; 
	_float      m_fSpeed = 0.f;

public:
	static CEffectSquare* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOriginPos, int numParticles, ParticleBoundingBox* boundbox, const _tchar* pPath);

private:
	virtual void Free();
};