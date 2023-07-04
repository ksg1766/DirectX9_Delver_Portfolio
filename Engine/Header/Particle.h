#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CParticle :
    public CComponent
{
private:
    explicit CParticle();
    explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CParticle(const CParticle& rhs);
    virtual ~CParticle();

public:
	HRESULT			Ready_Particle();
	virtual _int	Update_Component(const _float& fTimeDelta);

protected:
	_vec3	m_vPos;

public:
	static	CParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void);
private:
	virtual void			Free();
};

END