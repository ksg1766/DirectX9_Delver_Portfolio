#include "..\Header\GameManager.h"
#include "stdafx.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{

}

CGameManager::~CGameManager()
{
	Free();
}

_int CGameManager::Update_Game(const _float& fTimeDelta)
{
	switch (m_eCurr_PD)
	{
	case PD::Normal:
		break;
	case PD::ShowVillage:
		break;
	case PD::ShowSewer:
		break;
	case PD::ShowBoss:
		break;
	case PD::HekirekiIssen:
		HekirekiIssen(fTimeDelta);
		break;
	}

	return _int();
}

void CGameManager::LateUpdate_Game()
{
}

void CGameManager::Render_Game(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

void CGameManager::HekirekiIssen(const _float& fTimeDelta)	// 일단 절차지향으로 구현해보자
{
	if (PD::Normal == m_ePrev_PD && !m_bReadyBreath)
	{
		m_bReadyBreath = true;
		m_fTimer = 5.f;
		SceneManager()->SlowMode(m_fTimer, 4);
		m_ePrev_PD = m_eCurr_PD;

		// Sound
		

		return;
	}
	else if (m_bReadyBreath && PD::HekirekiIssen == m_ePrev_PD)
	{
		m_fTimer -= 4.f * fTimeDelta;

		if (m_fTimer < 0.f)
		{
			m_bReadyBreath = false;
			SceneManager()->SlowMode(0.f, 4);
			m_fTimer = 0.4f;
			return;
		}
		return;
	}

	if (m_fTimer > 0.f)
	{
		CTransform* pPlayerTrans = SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform;

		pPlayerTrans->Translate(12.f * 6.f * fTimeDelta * pPlayerTrans->m_vInfo[INFO_LOOK]);	// 플레이어 가속을 위함(테스트 필요)
		m_fTimer -= fTimeDelta;
	}
	else
	{
		m_ePrev_PD = PD::Normal;
		m_eCurr_PD = PD::Normal;
	}
}

void CGameManager::Free()
{
}
