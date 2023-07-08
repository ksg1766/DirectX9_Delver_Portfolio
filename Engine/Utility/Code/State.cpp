#include "..\..\Header\State.h"
#include "Transform.h"

CState::CState()
{
}

CState::CState(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_fChase(0.f)
	, m_vRandomPos(_vec3(0.f,0.f,0.f))
{
	m_pGraphicDev->AddRef();
}

CState::~CState()
{

}

void CState::Free()
{
	Safe_Release<LPDIRECT3DDEVICE9>(m_pGraphicDev);
}
