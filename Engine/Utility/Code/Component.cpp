#include "Export_Utility.h"

CComponent::CComponent()
	: m_pGraphicDev(nullptr), m_bClone(false), m_pHost(nullptr)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bClone(false), m_pHost(nullptr)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_bClone(true), m_pHost(rhs.m_pHost)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CComponent::~CComponent()
{
}

void CComponent::Init_Property(CGameObject * _pHost)
{
	m_pHost = _pHost;
	m_eObjectTag = m_pHost->Get_ObjectTag();

	if(m_eObjectTag == OBJECTTAG::BLOCK || m_eObjectTag == OBJECTTAG::DOORBLOCK)
		m_pTransform = static_cast<CTransform*>(m_pHost->Get_Component(COMPONENTTAG::TRANSFORM, ID_STATIC));
	else
		m_pTransform = static_cast<CTransform*>(m_pHost->Get_Component(COMPONENTTAG::TRANSFORM, ID_DYNAMIC));
}

void CComponent::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
