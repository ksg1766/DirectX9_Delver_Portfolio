#include "stdafx.h"
#include "..\Header\GameManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "SkeletonKing.h"
#include "CameraManager.h"
#include "FlyingCamera.h"

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{
	if (!m_pPlayer)
		m_pPlayer = SceneManager()->Get_Scene()->Get_MainPlayer();
	if (!m_pBoss)
		m_pBoss = static_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front());
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
	case PD::HekirekiIssen_SideView:
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
			m_fTimer = 0.5f;

			return;
		}
		return;
	}

	if (PD::HekirekiIssen == m_eCurr_PD && m_fTimer > 0.f)
	{
		CTransform* pPlayerTrans = m_pPlayer->m_pTransform;
		
		pPlayerTrans->Translate(20.f * 6.f * fTimeDelta * pPlayerTrans->m_vInfo[INFO_LOOK]);	// 플레이어 가속을 위함(테스트 필요)
		m_fTimer -= fTimeDelta;
	}
	else if(PD::HekirekiIssen == m_eCurr_PD && m_fTimer <= 0.f)
	{
		m_pPlayer->Get_RigidBody()->UseGravity(true);
		m_ePrev_PD = m_eCurr_PD;
		m_eCurr_PD = PD::HekirekiIssen_SideView;
	}

	if (PD::HekirekiIssen_SideView == m_eCurr_PD && PD::HekirekiIssen == m_ePrev_PD && m_fTimer < 0.f)
	{
		// 사이드 뷰로 전환 후 이펙트
		m_pPlayer->m_pTransform->Translate(2.f * (m_pBoss->m_pTransform->m_vInfo[INFO_POS] - m_pPlayer->m_pTransform->m_vInfo[INFO_POS]));
		
		// Y값 수정 해 줘야함. 이거 말고 강제로 돌려서 고정시킬 방법이 있지 않을까...

		m_fTimer = 10.f;
		SceneManager()->SlowMode(m_fTimer, 5);

		static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();
		m_ePrev_PD = m_eCurr_PD;
		
		return;
	}

	if (PD::HekirekiIssen_SideView == m_eCurr_PD && PD::HekirekiIssen_SideView == m_ePrev_PD)
	{
		CFlyingCamera* pCamera = static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());

		if (m_fTimer > 8.f)  
		{
			// 사이드 뷰로 전환
			m_fTimer -= fTimeDelta;
			pCamera->m_pTransform->RotateAround(m_pBoss->m_pTransform->m_vInfo[INFO_POS], m_pBoss->m_pTransform->m_vInfo[INFO_UP], D3DXToRadian(0.5f * 45.f * fTimeDelta));
			D3DXVec3Normalize(&pCamera->m_pTransform->m_vInfo[INFO_LOOK],
				&(m_pBoss->m_pTransform->m_vInfo[INFO_POS] - pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Cross(&pCamera->m_pTransform->m_vInfo[INFO_RIGHT], &_vec3(0.f, 1.f, 0.f), &pCamera->m_pTransform->m_vInfo[INFO_LOOK]);
			D3DXVec3Cross(&pCamera->m_pTransform->m_vInfo[INFO_UP], &pCamera->m_pTransform->m_vInfo[INFO_LOOK], &pCamera->m_pTransform->m_vInfo[INFO_RIGHT]);

			/*pCamera->m_pTransform->RotateAround(pCamera->m_pTransform->m_vInfo[INFO_POS], pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXToRadian(5 * 30.f * fTimeDelta));*/
		}
		else
		{
			//pCamera->Change_Mode();
		}
	}
}

void CGameManager::Free()
{
}
