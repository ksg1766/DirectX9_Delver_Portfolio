#include "..\..\Header\Particle.h"

CParticle::CParticle()
{
}

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_vPos(_vec3())
{
}

CParticle::CParticle(const CParticle& rhs)
	: CComponent(rhs), m_vPos(rhs.m_vPos)
{
}

CParticle::~CParticle()
{
}

HRESULT CParticle::Ready_Particle()
{


	return S_OK;
}

_int CParticle::Update_Component(const _float& fTimeDelta)
{
	return _int();
}

CParticle* CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle* pInstance = new CParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Particle Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CParticle::Clone(void)
{
	return new CParticle(*this);
}

void CParticle::Free()
{
	CComponent::Free();
}
