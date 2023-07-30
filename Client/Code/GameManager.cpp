#include "stdafx.h"
#include "..\Header\GameManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "SkeletonKing.h"

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{
	if (!m_pPlayer)
		m_pPlayer = SceneManager()->Get_Scene()->Get_MainPlayer();
	/*if (!m_pBoss)
		m_pBoss = static_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front());*/
}

CGameManager::~CGameManager()
{
	Free();
}

_int CGameManager::Update_Game(const _float& fTimeDelta)
{
	/*if (!m_pPlayer)
		m_pPlayer = SceneManager()->Get_Scene()->Get_MainPlayer();*/

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
		m_fTimer = 15.5f;
		SceneManager()->SlowMode(m_fTimer, 5);
		m_ePrev_PD = m_eCurr_PD;

		// Sound
		CSoundManager::GetInstance()->StopAll();
		CSoundManager::GetInstance()->PlaySound(L"HekiRekiIssen", SOUND_HEKIREKIISSEN, 1.f);

		return;
	}
	else if (m_bReadyBreath && PD::HekirekiIssen == m_ePrev_PD)
	{
		m_fTimer -= 5.f * fTimeDelta;

		if (m_fTimer < 0.f)
		{
			m_bReadyBreath = false;
			m_pPlayer->Get_RigidBody()->UseGravity(false);
			SceneManager()->SlowMode(0.f, 5);
			m_fTimer = 1.8f;

			return;
		}
		return;
	}

	if (m_fTimer > 0.f)
	{
		CTransform* pPlayerTrans = m_pPlayer->m_pTransform;
		
		pPlayerTrans->Translate(12.f * 6.f * fTimeDelta * pPlayerTrans->m_vInfo[INFO_LOOK]);	// 플레이어 가속을 위함(테스트 필요)
		m_fTimer -= fTimeDelta;
	}
	else
	{
		m_pPlayer->Get_RigidBody()->UseGravity(true);
		m_ePrev_PD = m_eCurr_PD;
		m_eCurr_PD = PD::HekirekiIssen_SideView;
	}

	if (PD::HekirekiIssen_SideView == m_eCurr_PD && PD::HekirekiIssen == m_ePrev_PD && m_fTimer < 0.f)
	{
		// 사이드 뷰로 전환 후 이펙트
		m_pPlayer->m_pTransform->m_vInfo[INFO_POS] = m_pBoss->m_pTransform->m_vInfo[INFO_POS]
			+ 20.f * (m_pBoss->m_pTransform->m_vInfo[INFO_LOOK] - _vec3(0.f, m_pBoss->m_pTransform->m_vInfo[INFO_LOOK].y, 0.f));
		// Y값 수정 해 줘야함. 이거 말고 강제로 돌려서 고정시킬 방법이 있지 않을까...

		m_fTimer = 5.f;
		SceneManager()->SlowMode(m_fTimer, 5);
	}

	if (PD::HekirekiIssen_SideView == m_eCurr_PD && PD::HekirekiIssen == m_ePrev_PD && m_fTimer > 0.f)
	{
		m_fTimer = 2.f;
		SceneManager()->SlowMode(m_fTimer, 5);
	
		// 사이드 뷰로 전환
		SceneManager()->Get_Scene()->Get_ObjectList(LAYERTAG::ENVIRONMENT, OBJECTTAG::CAMERA).front()
			->m_pTransform->RotateAround(m_pBoss->m_pTransform->m_vInfo[INFO_POS], m_pBoss->m_pTransform->m_vInfo[INFO_UP], D3DXToRadian(30.f * fTimeDelta * 2.f));
	}

	if (PD::HekirekiIssen_SideView == m_eCurr_PD && PD::HekirekiIssen == m_ePrev_PD && m_fTimer < 0.f)
	{

	}
}

void CGameManager::Free()
{
}
