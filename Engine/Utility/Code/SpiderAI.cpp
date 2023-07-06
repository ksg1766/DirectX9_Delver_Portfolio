#include "..\..\Header\SpiderAI.h"
#include "Transform.h"

CSpiderAI::CSpiderAI()
{
}

CSpiderAI::CSpiderAI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonsterAI(pGraphicDev), m_fChase(0.f), m_bIsJumping(false)
{
}

CSpiderAI::CSpiderAI(const CSpiderAI& rhs)
	: CMonsterAI(rhs), m_fChase(rhs.m_fChase), m_bIsJumping(rhs.m_bIsJumping)
{
}

CSpiderAI::~CSpiderAI()
{
}

HRESULT CSpiderAI::Ready_SpiderAI()
{
	m_eState = STATE::Roming;
	m_fJumpInitializeVelocity = 15.f;

	return S_OK;
}

_int CSpiderAI::Update_Component(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Component(fTimeDelta);


	return iExit;
}

void CSpiderAI::Jump(const _float& fTimeDelta)
{
	if (!m_bIsJumping)
		return;

	m_pTransform->m_vInfo[INFO_POS].y += m_fJumpInitializeVelocity * fTimeDelta;

	m_fJumpInitializeVelocity -= 0.5f * fTimeDelta * fTimeDelta * 3000.f;

	if (m_pTransform->m_vInfo[INFO_POS].y < 1.f)
	{
		m_bIsJumping = false;
		m_pTransform->m_vInfo[INFO_POS].y = 1.f;
		m_fJumpInitializeVelocity = 20.f;
	}
	
}

CSpiderAI* CSpiderAI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpiderAI*	pInstance = new CSpiderAI(pGraphicDev);

	if (FAILED(pInstance->Ready_SpiderAI()))
	{
		Safe_Release<CSpiderAI*>(pInstance);

		MSG_BOX("Create SpiderAI Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CSpiderAI::Clone()
{
	return new CSpiderAI(*this);
}

void CSpiderAI::Free()
{
	__super::Free();
}
