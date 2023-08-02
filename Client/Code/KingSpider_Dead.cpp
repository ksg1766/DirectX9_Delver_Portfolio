#include "KingSpider_Dead.h"
#include "KingSpider.h"
#include "Export_Function.h"
CKingSpider_Dead::CKingSpider_Dead()
{
}

CKingSpider_Dead::CKingSpider_Dead(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_Dead::~CKingSpider_Dead()
{
}

HRESULT CKingSpider_Dead::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	return S_OK;
}

STATE CKingSpider_Dead::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;
	if (10.f < m_fDelay)
		m_pOwner->Get_Host()->Set_Dead(true);

	return STATE::BOSS_DEAD;
}

void CKingSpider_Dead::LateUpdate_State()
{
}

void CKingSpider_Dead::Render_State()
{
}

CKingSpider_Dead* CKingSpider_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_Dead* pState = new CKingSpider_Dead(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_Dead*>(pState);

		MSG_BOX("KingSpider_Appear Failed");
		return nullptr;
	}
	return pState;
}

void CKingSpider_Dead::Free()
{
	__super::Free();
}
