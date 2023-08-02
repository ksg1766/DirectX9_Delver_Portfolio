#include "stdafx.h"
#include "Export_Function.h"
#include "SoundManager.h"
#include "Boss_LastPhase.h"
#include "SkeletonKing.h"
#include "Player.h"
#include "GameManager.h"
CBoss_LastPhase::CBoss_LastPhase()
{
}

CBoss_LastPhase::CBoss_LastPhase(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_LastPhase::~CBoss_LastPhase()
{
}

HRESULT CBoss_LastPhase::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fHekiRekiDelay = 0.f;
	return S_OK;
}

STATE CBoss_LastPhase::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;
	if ((PD::HekirekiIssen != CGameManager::GetInstance()->Get_PlayMode())&& (4.f < m_fDelay))
	{
		m_vDir = SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS] - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pOwner->Get_Transform()->Translate(m_vDir);
		m_fDelay = 0.f;
	}

	if (PD::HekirekiIssen == CGameManager::GetInstance()->Get_PlayMode())
	{
		m_fHekiRekiDelay += fTimeDelta;
		if (15.f <= m_fHekiRekiDelay)
		{
			return STATE::BOSS_DYING;
		}
	}
	return STATE::BOSS_CRAWL;
}

void CBoss_LastPhase::LateUpdate_State()
{
}

void CBoss_LastPhase::Render_State()
{
}

CBoss_LastPhase* CBoss_LastPhase::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBoss_LastPhase* pState = new CBoss_LastPhase(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBoss_LastPhase*>(pState);

		MSG_BOX("Boss LastPhase State Failed");
	}
	return pState;
}

void CBoss_LastPhase::Free()
{
	__super::Free();
}
