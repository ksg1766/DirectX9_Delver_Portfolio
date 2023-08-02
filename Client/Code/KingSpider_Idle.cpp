#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_Idle.h"
#include "Export_Function.h"
#include "Player.h"
#include "KingSpider.h"
#include "FlyingCamera.h"
#include "CameraManager.h"

CKingSpider_Idle::CKingSpider_Idle()
{
}

CKingSpider_Idle::CKingSpider_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_Idle::~CKingSpider_Idle()
{
}

HRESULT CKingSpider_Idle::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_iSkillCount = 0.f;
	m_bJumpAttack = false;
	m_bJumpRun = false;
	m_vFogPos[0] = _vec3(4.5f, 0.f, 0.f);
	m_vFogPos[1] = _vec3(-4.5f, 0.f, 0.f);
	m_vFogPos[2] = _vec3(0., 0.f, 4.5f);
	m_vFogPos[3] = _vec3(0., 0.f, -4.5f);
	m_vFogPos[4] = _vec3(4.5f, 0.f, -4.5f);
	m_vFogPos[5] = _vec3(-4.5f, 0.f, 4.5f);
	m_vFogPos[6] = _vec3(-4.5f, 0.f, -4.5f);
	m_vFogPos[7] = _vec3(4.5f, 0.f, 4.5f);

	return S_OK;
}

STATE CKingSpider_Idle::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;
	/*if (0 >= dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fHP)
		return STATE::BOSS_DEAD;*/
	if ((!m_bJumpRun)&&(0 >= dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fHP))
	{
		m_bJumpRun = true;
		return STATE::BOSS_TELEPORT;
	}

	if (3.f < m_fDelay)
	{
		m_fDelay = 0.f;
		if (BOSSPHASE::PHASE1 == dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_Phase())
		{
			switch (m_iSkillCount)
			{
			case 0:
				++m_iSkillCount;
				return STATE::BOSS_PH1SKILL1;
				break;

			case 1:
				++m_iSkillCount;
				return STATE::BOSS_PH1SKILL2;
				break;

			case 2:
				m_iSkillCount = 0;
				return STATE::BOSS_PH1SKILL3;
				break;
			}
		}

		else if (BOSSPHASE::PHASE2 == dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_Phase()) 
		{
			switch (m_iSkillCount)
			{
			case 0:
				++m_iSkillCount;
				return STATE::BOSS_PH1SKILL1;
				break;

			case 1:
				m_iSkillCount = 0;
				return STATE::BOSS_PH1SKILL2;
				break;
			}
		}
	}

	return STATE::BOSS_IDLE;
}

void CKingSpider_Idle::LateUpdate_State()
{
}

void CKingSpider_Idle::Render_State()
{
}

CKingSpider_Idle* CKingSpider_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_Idle* pState = new CKingSpider_Idle(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_Idle*>(pState);

		MSG_BOX("KingSpider_Idle Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_Idle::Free()
{
	__super::Free();
}
