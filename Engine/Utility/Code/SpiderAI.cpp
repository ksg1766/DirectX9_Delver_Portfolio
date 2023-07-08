#include "..\..\Header\SpiderAI.h"
#include "Transform.h"
#include "StateMachine.h"

CSpiderAI::CSpiderAI()
{
}

CSpiderAI::CSpiderAI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonsterAI(pGraphicDev)
{
}

CSpiderAI::CSpiderAI(const CSpiderAI& rhs)
	: CMonsterAI(rhs)
{
}

CSpiderAI::~CSpiderAI()
{
}

HRESULT CSpiderAI::Ready_SpiderAI()
{
	m_eState = STATE::ROMIMG;
	m_fJumpInitializeVelocity = 15.f;
	m_fChase = 0.f;
	m_bIsJumping = false;
	return S_OK;
}

_int CSpiderAI::Update_Component(const _float& fTimeDelta, const _vec3 _PlayerPos)
{
	_int iExit = __super::Update_Component(fTimeDelta);

	m_pStateMachine->Update_StateMachine(fTimeDelta);

	//if (m_eState == STATE::ROMIMG && m_fChase < 5.f)
	//{
	//	m_fSpeed = 5.f;
	//	m_fChase += fTimeDelta;
	//	
	//	if (m_fChase >= 3.f)
	//	{
	//		m_eState = STATE::ATTACK;
	//		m_fChase = 0.f;
	//		m_bIsJumping = true;
	//	}

	//}

	//if (m_eState == STATE::ATTACK)
	//{
	//	m_fSpeed = 13.f;
	//	Jump(fTimeDelta);

	//	if (!m_bIsJumping)
	//	{
	//		m_eState = STATE::ROMIMG;
	//	}
	//}

	//Chase_Target(&_PlayerPos, fTimeDelta, m_fSpeed);

	

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
