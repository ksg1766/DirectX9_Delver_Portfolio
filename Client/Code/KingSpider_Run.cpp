#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_Run.h"
#include "Export_Function.h"
#include "Player.h"
#include "KingSpider.h"
CKingSpider_Run::CKingSpider_Run()
{
}

CKingSpider_Run::CKingSpider_Run(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_Run::~CKingSpider_Run()
{
}

HRESULT CKingSpider_Run::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bRunTrigger = false;
	m_bJumpTrigger = false;
	m_bJumpCheck = false;
	m_fDelay = 0.f;
	return S_OK;
}

STATE CKingSpider_Run::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;
	if (!m_bRunTrigger)
	{
		m_vDir = _vec3(-30.f, 20.f, 15.f) - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		//D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_bRunTrigger = true;
		m_bJumpTrigger = true;
		m_fDelay = 0.f;
	}
	if (m_bJumpTrigger)
	{
		dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->Add_Force(_vec3(m_vDir.x, 30.f, m_vDir.z));
		dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(true);
		m_bJumpTrigger = false;
		m_bJumpCheck = true;
		m_fDelay = 0.f;
	}
	if ((m_bRunTrigger) && (!m_bJumpTrigger) && (dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_FloorCollison()))
	{
		dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(false);
		return STATE::BOSS_TELEPORT;
	}

}

void CKingSpider_Run::LateUpdate_State()
{
}

void CKingSpider_Run::Render_State()
{
}

CKingSpider_Run* CKingSpider_Run::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_Run* pState = new CKingSpider_Run(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_Run*>(pState);

		MSG_BOX("KingSpider_Run Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_Run::Free()
{
	__super::Free();
}
