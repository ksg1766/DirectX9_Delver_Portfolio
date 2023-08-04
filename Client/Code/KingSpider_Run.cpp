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
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_bRunTrigger = true;
		m_bJumpTrigger = true;
		m_fDelay = 0.f;
	}
	if (m_bJumpTrigger)
	{
		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BOSS);
		CSoundManager::GetInstance()->PlaySound(L"KingSpider_Spawn.mp3", CHANNELID::SOUND_BOSS, 1.f);

		static_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(true);
		static_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->Add_Force(_vec3(0.f, 25.f, 0.f));
		m_bJumpTrigger = false;
		m_bJumpCheck = true;
		m_fDelay = 0.f;
	}
	if ((m_bJumpCheck) && (1.5f >= m_fDelay))
		m_pOwner->Get_Transform()->Translate(_vec3(m_vDir.x, 0.f, m_vDir.z));

	if ((m_bJumpCheck)&&(2.f < m_fDelay))
	{
		if(73.f >= m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y)
			m_pOwner->Get_Transform()->Translate(_vec3(0.f, 1.5f*fTimeDelta, 0.f));
		
		if (0 >= static_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_CrawlingHP())
		{
			static_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(true);
			return STATE::BOSS_DEAD;
		}
		
		if (72.5f <= m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y)
		{
			static_cast<CKingSpider*>(m_pOwner->Get_Host())->Set_Dead(true);
			//m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = _vec3(6.f, 75.f, 67.f);
			//static_cast<CKingSpider*>(m_pOwner->Get_Host())->Set_Phase(BOSSPHASE::PHASE2);
			//return STATE::BOSS_IDLE;
		}

	}

	if(static_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_FloorCollison())
		static_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(false);

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
