#include "stdafx.h"
#include "..\Header\GameManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "SkeletonKing.h"
#include "CameraManager.h"
#include "FlyingCamera.h"
#include "BlackIn.h"
//#include "KingSpider.h"

#include "EffectSwordTrail.h"
#include "EffectSwordLightning.h"
#include "EffectSwordParticles.h"
#include <BlackOutIn.h>
#include <UICredit.h>

IMPLEMENT_SINGLETON(CGameManager)

CGameManager::CGameManager()
{
	//if (!m_pPlayer)
	//	m_pPlayer = SceneManager()->Get_Scene()->Get_MainPlayer();
	
	//if(SCENETAG::BOSSSTAGE == SceneManager()->Get_Scene()->Get_SceneTag())
	//if (!m_pBoss)
	//	m_pBoss = static_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front());
}

CGameManager::~CGameManager()
{
	Free();
}

_int CGameManager::Update_Game(const _float& fTimeDelta)
{
	if (!m_pPlayer)
		m_pPlayer = SceneManager()->Get_Scene()->Get_MainPlayer();

	if (!m_pCamera)
		m_pCamera = dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());

	if (!m_pBoss &&SCENETAG::BOSSSTAGE == SceneManager()->Get_Scene()->Get_SceneTag())
		m_pBoss = static_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front());

	if (PD::Normal != m_eCurr_PD)
	{
		CInputDev::GetInstance()->Lock_Input(true);
		switch (m_eCurr_PD)
		{
		case PD::ShowVillage:
			ShowVillage(fTimeDelta);
			break;
		case PD::ShowSewer:
			ShowSewer(fTimeDelta);
			break;
		case PD::ShowTower:
			ShowTower(fTimeDelta);
			break;
		case PD::ShowMiniBoss:
			ShowMiniBoss(fTimeDelta);
			break;
		case PD::ShowBoss:
			ShowBoss(fTimeDelta);
			break;
		case PD::ShowBossP2:
			ShowBossP2(fTimeDelta);
			break;
		case PD::ShowBossP3:
			ShowBossP3(fTimeDelta);
			break;
		case PD::MeteorExplosion:
			CInputDev::GetInstance()->Lock_Input(false);
			MeteorExplosion(fTimeDelta);
			break;
		case PD::ShowBossClone:
			ShowBossClone(fTimeDelta);
			break;
		case PD::HekirekiIssen:
		case PD::HekirekiIssen_SideView:
			CInputDev::GetInstance()->Lock_Input(false);
			HekirekiIssen(fTimeDelta);
			break;
		case PD::ClearGame:
			ClearGame(fTimeDelta);
			break;
		}
	}

	return _int();
}

void CGameManager::LateUpdate_Game()
{
}

void CGameManager::Render_Game(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

void CGameManager::ShowVillage(const _float& fTimeDelta)
{
	if (!m_iVisitCount && m_fTimer == 10.f)
	{
		static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();
		m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-19.5f, 16.f, -62.f);
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-22.f, 16.f, -71.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

		++m_iVisitCount;
	}
	else if (1 == m_iVisitCount)
	{
		if (m_fTimer > 3.f)
		{
			m_pCamera->m_pTransform->Translate(fTimeDelta * -m_pCamera->m_pTransform->m_vInfo[INFO_UP]);
		}
		else
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-5.f, 7.f, -30.f);
			// 수치 입력해야 함. 임의 수치.
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(0.f, 3.f, -22.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if(2 == m_iVisitCount)
	{
		if(m_fTimer > -2.f)
		{
			// Action
			m_pCamera->m_pTransform->RotateAround(_vec3(0.f, 3.f, -22.f), _vec3(0.f, 1.f, 0.f), -0.3f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(0.f, 3.f, -22.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(35.f, 20.f, -45.f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(0.f, -10.f, 0.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
			
			++m_iVisitCount;
		}
	}
	else if (3 == m_iVisitCount)
	{
		if (m_fTimer > -9.f)
		{
			// Action
			m_pCamera->m_pTransform->RotateAround(_vec3(0.f, -10.f, 0.f), _vec3(0.f, 1.f, 0.f), 0.1f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(0.f, -10.f, 0.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			m_iVisitCount = 0;

			static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();

			m_fTimer = 10.f;
			m_eCurr_PD = PD::Normal;
			m_ePrev_PD = PD::Normal;

			CInputDev::GetInstance()->Lock_Input(false);

			return;
		}
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::ShowSewer(const _float& fTimeDelta)
{
	if (!m_iVisitCount && m_fTimer == 10.f)
	{
		static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();
		m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(32.05f, 14.01f, -76.38f);
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(35.13f, 09.59f, -82.84f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

		++m_iVisitCount;
	}
	else if (1 == m_iVisitCount)
	{
		if (m_fTimer <= 5.f)
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-38.86f, 16.09f, -80.15f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-54.52f, 11.11f, -93.26f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (2 == m_iVisitCount)
	{
		if (m_fTimer > -1.f)
		{
			m_pCamera->m_pTransform->Translate(0.5f * fTimeDelta * _vec3(0.f, -1.f, 0.f));
		}
		else
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-19.79f, 76.00f, -27.22f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(32.47f, 167.78f, 31.17f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (3 == m_iVisitCount)
	{
		if (m_fTimer > -7.f)
		{
			// Action
			m_pCamera->m_pTransform->RotateAround(_vec3(32.47f, 167.78f, 31.17f), _vec3(0.f, 1.f, 0.f), 0.2f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(32.47f, 167.78f, 31.17f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-26.13f, 17.30f, 4.75f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-34.97f, 14.01f, 15.04f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (4 == m_iVisitCount)
	{
		if (m_fTimer > -14.f)
		{
			m_pCamera->m_pTransform->Translate(-20.f * fTimeDelta * m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]);
		}
		else
		{
			m_iVisitCount = 0;

			static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();

			m_fTimer = 10.f;
			m_eCurr_PD = PD::Normal;
			m_ePrev_PD = PD::Normal;

			CInputDev::GetInstance()->Lock_Input(false);

			return;
		}
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::ShowTower(const _float& fTimeDelta)
{
	if (m_fTimer == 10.f)
	{
		m_pCamera->Change_Mode();
		m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(48.f, 72.f, 47.5f);
	}
	else if (m_fTimer > 3.f)
	{
		CCameraManager::GetInstance()->LookAtTarget(_vec3(32.f, 194.f, 34.f), fTimeDelta);
		m_pCamera->m_pTransform->Translate(4.f * fTimeDelta * _vec3(0.f, 1.f, 0.f));
		m_pCamera->m_pTransform->RotateAround(_vec3(32.f, 77.f, 34.f), _vec3(0.f, 1.f, 0.f), 0.5f * fTimeDelta);
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(32.f, 194.f, 34.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
	}
	else if (m_fTimer > 0.5f)
	{
		CCameraManager::GetInstance()->ZoomOutToTrans(m_pPlayer->m_pTransform, fTimeDelta);
	}
	else
	{
		static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();

		m_pCamera->m_pTransform->Copy_RUL_AddPos(m_pPlayer->m_pTransform->m_vInfo);
		m_fTimer = 10.f;
		m_eCurr_PD = PD::Normal;
		m_ePrev_PD = PD::Normal;

		CInputDev::GetInstance()->Lock_Input(false);

		return;
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::ShowBoss(const _float& fTimeDelta)
{
	if (!m_iVisitCount && m_fTimer == 10.f)
	{
		static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();
		m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-30.f, 37.f, 93.5f);
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-70.f, 33.5f, 93.5f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

		++m_iVisitCount;
	}
	else if (1 == m_iVisitCount)
	{
		if (m_fTimer >= 5.f)
		{
			m_pCamera->m_pTransform->RotateAround(_vec3(-70.f, 33.5f, 93.5f), _vec3(0.f, 1.f, 0.f), 0.5f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-70.f, 33.5f, 93.5f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-110.f, 37.f, -85.5f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-70.f, 33.5f, -85.5f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (2 == m_iVisitCount)
	{
		if (m_fTimer > -1.f)
		{
			m_pCamera->m_pTransform->RotateAround(_vec3(-70.f, 33.5f, -85.5f), _vec3(0.f, 1.f, 0.f), -0.5f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-70.f, 33.5f, -85.5f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-50.f, 40.f, 0.f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-80.f, 35.f, 0.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (3 == m_iVisitCount)
	{
		if (m_fTimer > -7.f)
		{
			m_pCamera->m_pTransform->RotateAround(_vec3(-80.f, 35.f, 0.f), _vec3(0.f, 1.f, 0.f), D3DXToRadian(60.f * fTimeDelta));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-80.f, 35.f, 0.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(62.f, 21.f, 1.f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(62.f, 21.f, 1.f) - _vec3(200.f, 40.f, 1.f)));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (4 == m_iVisitCount)
	{
		if (m_fTimer > -15.5f)
		{
			m_pCamera->m_pTransform->Translate(-20.f * fTimeDelta * m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]);
		}
		else
		{
			m_iVisitCount = 0;

			static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();

			m_fTimer = 10.f;
			m_eCurr_PD = PD::Normal;
			m_ePrev_PD = PD::Normal;

			CInputDev::GetInstance()->Lock_Input(false);

			return;
		}
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::ShowBossP2(const _float& fTimeDelta)
{
	CGameObject* pBoss = SceneManager()->Get_Scene()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front();

	if (m_fTimer > 7.f)
	{
		CCameraManager::GetInstance()->LookAtTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta);
		CCameraManager::GetInstance()->ZoomInTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta, 1.2f);
	}
	else if (m_fTimer > 6.0f)
	{
		CCameraManager::GetInstance()->ZoomOutToTrans(m_pPlayer->m_pTransform, fTimeDelta);
	}
	else if (m_fTimer > 3.8f)
	{
		CCameraManager::GetInstance()->LookAtTarget(_vec3(-72.f, 34.f, 33.f), fTimeDelta);
		CCameraManager::GetInstance()->ZoomInTarget(_vec3(-72.f, 34.f ,33.f), fTimeDelta, 1.5f);
	}
	else if (m_fTimer > 2.3f)
	{
		CCameraManager::GetInstance()->ZoomOutToTrans(m_pPlayer->m_pTransform, fTimeDelta);
	}
	else
	{
		m_pCamera->m_pTransform->Copy_RUL_AddPos(m_pPlayer->m_pTransform->m_vInfo);
		m_fTimer = 10.f;
		m_eCurr_PD = PD::Normal;
		m_ePrev_PD = PD::Normal;

		CInputDev::GetInstance()->Lock_Input(false);

		return;
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::ShowBossP3(const _float& fTimeDelta)
{
	CGameObject* pBoss = SceneManager()->Get_Scene()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front();

	if (m_fTimer > 8.5f)
	{
		m_pCamera->m_pTransform->Translate(0.1f * fTimeDelta * m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]);
		CCameraManager::GetInstance()->ZoomInTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta, 1.2f);
		CCameraManager::GetInstance()->LookAtTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta);
	}
	else if (m_fTimer > 4.0f)
	{
		m_pCamera->m_pTransform->RotateAround(pBoss->m_pTransform->m_vInfo[INFO_POS], _vec3(0.f, 1.f, 0.f), -1.4f * fTimeDelta);
		CCameraManager::GetInstance()->LookAtTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta);
	}
	else if (m_fTimer > 2.5f)
	{
		CCameraManager::GetInstance()->ZoomOutToTrans(m_pPlayer->m_pTransform, fTimeDelta);
	}
	else
	{
		m_pCamera->m_pTransform->Copy_RUL_AddPos(m_pPlayer->m_pTransform->m_vInfo);
		m_fTimer = 10.f;
		m_eCurr_PD = PD::Normal;
		m_ePrev_PD = PD::Normal;

		CInputDev::GetInstance()->Lock_Input(false);

		return;
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::ShowMiniBoss(const _float& fTimeDelta)
{
	CGameObject* pBoss = SceneManager()->Get_Scene()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front();
	
	if (m_fTimer == 10.f)
	{
		m_pCamera->Change_Mode();
		//m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(48.f, 72.f, 47.5f);
	}
	if (m_fTimer > 7.f)
	{
	}
	else if (m_fTimer > 5.5f)
	{
		CCameraManager::GetInstance()->LookAtTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta);
	}
	else if (m_fTimer > 4.5f)
	{
		CCameraManager::GetInstance()->ZoomInTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta, 1.2f);
	}
	else if (m_fTimer > 3.1f)
	{
		CCameraManager::GetInstance()->LookAtTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta);
	}
	else if (m_fTimer > 2.1f)
	{
		CCameraManager::GetInstance()->ZoomOutToTrans(m_pPlayer->m_pTransform, fTimeDelta);
	}
	else
	{
		m_pCamera->Change_Mode();
		m_pCamera->m_pTransform->Copy_RUL_AddPos(m_pPlayer->m_pTransform->m_vInfo);
		m_fTimer = 10.f;
		m_eCurr_PD = PD::Normal;
		m_ePrev_PD = PD::Normal;

		CInputDev::GetInstance()->Lock_Input(false);

		return;
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::MeteorExplosion(const _float& fTimeDelta)
{
	m_bPlayOnce = false;

	CGameObject* pBoss = SceneManager()->Get_Scene()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front();

	if (m_fTimer == 10.f)
	{
		SceneManager()->SlowMode(2.f, 2.f);
		CSoundManager::GetInstance()->SetChannelVolume(SOUND_BGM, 0.2f);
		CSoundManager::GetInstance()->PlaySound(L"BassDrop", SOUND_EFFECT2, 2.0f);
	}
	else if (m_fTimer > 7.5f)
	{
		m_fTimer -= fTimeDelta;
	}
	else
	{
		m_pCamera->m_pTransform->Copy_RUL_AddPos(m_pPlayer->m_pTransform->m_vInfo);
		m_fTimer = 10.f;
		m_eCurr_PD = PD::Normal;
		m_ePrev_PD = PD::Normal;

		CSoundManager::GetInstance()->SetChannelVolume(SOUND_BGM, 1.f);

		return;
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::ShowBossClone(const _float& fTimeDelta)
{
	if (m_fTimer == 10.f)
	{
		m_pCamera->Change_Mode();
	}
	if (m_fTimer > 9.2f)
	{
		CCameraManager::GetInstance()->LookAtTarget(_vec3(-72.5f, 49.f, 124.f), 4.f * fTimeDelta);
		CCameraManager::GetInstance()->ZoomInTarget(_vec3(-72.5f, 49.f, 124.f), fTimeDelta, 1.05f);
	}
	else if (m_fTimer > 8.4f)
	{
		CCameraManager::GetInstance()->LookAtTarget(_vec3(-104.f, 49.f, 94.5f), 4.f * fTimeDelta);
	}
	else if (m_fTimer > 7.6f)
	{
		CCameraManager::GetInstance()->LookAtTarget(_vec3(-38.f, 49.f, 94.5f), 4.f * fTimeDelta);
	}
	else if (m_fTimer > 7.f)
	{
		CCameraManager::GetInstance()->ZoomOutToTrans(m_pPlayer->m_pTransform, fTimeDelta);
	}
	else
	{
		m_pCamera->Change_Mode();
		m_pCamera->m_pTransform->Copy_RUL_AddPos(m_pPlayer->m_pTransform->m_vInfo);
		m_fTimer = 10.f;
		m_eCurr_PD = PD::Normal;
		m_ePrev_PD = PD::Normal;

		CInputDev::GetInstance()->Lock_Input(false);

		return;
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::HekirekiIssen(const _float& fTimeDelta)	// 일단 절차지향으로 구현해보자
{
	if (PD::Normal == m_ePrev_PD && !m_bReadyBreath)
	{
		m_bReadyBreath = true;
		m_fTimer = 15.5f;
		SceneManager()->SlowMode(m_fTimer, 5);
		m_ePrev_PD = m_eCurr_PD;

		CSoundManager::GetInstance()->SetChannelVolume(SOUND_BGM, 0.15f);
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
		
		pPlayerTrans->Translate(23.f * 6.f * fTimeDelta * pPlayerTrans->m_vInfo[INFO_LOOK]);	// 플레이어 가속을 위함(테스트 필요)
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
		_vec3& vBossPos = m_pBoss->m_pTransform->m_vInfo[INFO_POS];
		_vec3& vPlayerPos = m_pPlayer->m_pTransform->m_vInfo[INFO_POS];
		_vec3 vOffset = m_pBoss->m_pTransform->m_vInfo[INFO_POS] - m_pPlayer->m_pTransform->m_vInfo[INFO_POS];
		m_pPlayer->m_pTransform->Translate(vOffset);
		vOffset.y = 0.f;
		D3DXVec3Normalize(&vOffset, &vOffset);
		m_pPlayer->m_pTransform->Translate(20.f * vOffset);

		m_fTimer = 5.f;
		SceneManager()->SlowMode(m_fTimer - 3.f, 3);

		m_pCamera->Change_Mode();
		m_ePrev_PD = m_eCurr_PD;
		
		return;
	}

	if (PD::HekirekiIssen_SideView == m_eCurr_PD && PD::HekirekiIssen_SideView == m_ePrev_PD)
	{
		if (m_fTimer > 4.f)
		{
			// 사이드 뷰로 전환
			m_fTimer -= 3.f * fTimeDelta;
			m_pCamera->m_pTransform->RotateAround(m_pBoss->m_pTransform->m_vInfo[INFO_POS], m_pBoss->m_pTransform->m_vInfo[INFO_UP], 3.f * D3DXToRadian(60.f * fTimeDelta));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(m_pBoss->m_pTransform->m_vInfo[INFO_POS] - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Cross(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]);
			D3DXVec3Cross(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]);
		}
		else if (m_fTimer > 3.f)
		{
			m_fTimer -= 3.f * fTimeDelta;
			if (!m_bEffectCreated[0])
			{
				LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
				CEffectSwordTrail* pSwordTrail = CEffectSwordTrail::Create(pGraphicDev);
				NULL_CHECK_RETURN(pSwordTrail);
				pSwordTrail->m_pTransform->m_vInfo[INFO_POS] = m_pBoss->m_pTransform->m_vInfo[INFO_POS];
				for (_uint i = 0; i < 3; ++i)
					CopyMemory(&pSwordTrail->m_pTransform->m_vInfo[i], &m_pBoss->m_pTransform->m_vInfo[2 - i], sizeof(_vec3));
				EventManager()->CreateObject(pSwordTrail, LAYERTAG::GAMELOGIC);
				m_bEffectCreated[0] = true;
			}
		}
		else if(m_fTimer > 2.f)
		{
			m_fTimer -= 1.f * fTimeDelta;
			if (!m_bEffectCreated[1])
			{
				LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
				CEffectSwordLightning* pSwordLightning = CEffectSwordLightning::Create(pGraphicDev);
				NULL_CHECK_RETURN(pSwordLightning);
				pSwordLightning->m_pTransform->m_vInfo[INFO_POS] = m_pBoss->m_pTransform->m_vInfo[INFO_POS];
				for (_uint i = 0; i < 3; ++i)
					CopyMemory(&pSwordLightning->m_pTransform->m_vInfo[i], &m_pBoss->m_pTransform->m_vInfo[2 - i], sizeof(_vec3));
				EventManager()->CreateObject(pSwordLightning, LAYERTAG::GAMELOGIC);
				m_bEffectCreated[1] = true;
			}
			if (!m_bEffectCreated[2])
			{
				LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
				CEffectSwordParticles* pSwordParticles = CEffectSwordParticles::Create(pGraphicDev);
				NULL_CHECK_RETURN(pSwordParticles);
				pSwordParticles->m_pTransform->m_vInfo[INFO_POS] = m_pBoss->m_pTransform->m_vInfo[INFO_POS];
				for (_uint i = 0; i < 3; ++i)
					CopyMemory(&pSwordParticles->m_pTransform->m_vInfo[i], &m_pBoss->m_pTransform->m_vInfo[2 - i], sizeof(_vec3));
				EventManager()->CreateObject(pSwordParticles, LAYERTAG::GAMELOGIC);
				m_bEffectCreated[2] = true;
			}
		}
		else if (m_fTimer > 0.5f)
		{
			m_fTimer -= fTimeDelta;
		}
		else
		{
			m_pBoss->Get_BasicStat()->Get_Stat()->fHP = 0.f;

			m_fTimer = 10.f;

			_vec3& vBossPos = m_pBoss->m_pTransform->m_vInfo[INFO_POS];
			_vec3& vPlayerPos = m_pPlayer->m_pTransform->m_vInfo[INFO_POS];
			_vec3 vOffset = m_pBoss->m_pTransform->m_vInfo[INFO_POS] - m_pPlayer->m_pTransform->m_vInfo[INFO_POS];
			m_pPlayer->m_pTransform->Translate(vOffset);
			vOffset.y = 0.f;
			D3DXVec3Normalize(&vOffset, &vOffset);

			m_pPlayer->m_pTransform->Translate(20.f * vOffset);

			m_pCamera->Change_Mode();
			m_eCurr_PD = PD::Normal;
			m_ePrev_PD = PD::Normal;

			CSoundManager::GetInstance()->SetChannelVolume(SOUND_BGM, 1.f);

			return;
		}
	}
}

void CGameManager::ClearGame(const _float& fTimeDelta)
{
	if (!m_iVisitCount && m_fTimer == 10.f)
	{
		static_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam())->Change_Mode();
		m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(26.f, 112.f, -39.f);
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(46.f, 27.5f, 3.5f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
		D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

		CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BGM);
		CSoundManager::GetInstance()->PlayBGM(L"Silian'sTheme", 1.f);

		++m_iVisitCount;
	}
	else if (1 == m_iVisitCount)
	{
		if (m_fTimer > 3.f)
		{
			m_pCamera->m_pTransform->Translate(5.f * fTimeDelta * -m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]);
			m_pCamera->m_pTransform->Translate(5.f * fTimeDelta * -m_pCamera->m_pTransform->m_vInfo[INFO_UP]);
		}
		else
		{
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.f, 100.f, -70.f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(0.f, 100.f, 0.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (2 == m_iVisitCount)
	{
		if (m_fTimer > -6.f)
		{
			m_pCamera->m_pTransform->Translate(10.f * fTimeDelta * -m_pCamera->m_pTransform->m_vInfo[INFO_UP]);
		}
		else
		{
			// 바드
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(8.f, 4.5f, -27.f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(5.f, 4.f, -22.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (3 == m_iVisitCount)
	{
		if (m_fTimer > -10.f)
		{
			m_pCamera->m_pTransform->RotateAround(_vec3(5.f, 4.f, -22.f), _vec3(0.f, 1.f, 0.f), 0.3f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(5.f, 4.f, -22.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			// 멍멍이
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.f, 4.5f, -23.f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-3.f, 4.f, -18.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (4 == m_iVisitCount)
	{
		if (m_fTimer > -14.f)
		{
			m_pCamera->m_pTransform->RotateAround(_vec3(-3.f, 4.f, -18.f), _vec3(0.f, 1.f, 0.f), 0.3f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-3.f, 4.f, -18.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			// 입구 유령
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-88.f, 4.f, -25.f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-94.f, 4.f, -23.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (5 == m_iVisitCount)
	{
		if (m_fTimer > -18.f)
		{
			m_pCamera->m_pTransform->RotateAround(_vec3(-94.f, 4.f, -23.f), _vec3(0.f, 1.f, 0.f), 0.3f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(-94.f, 4.f, -23.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			// 전체 보면서 공전
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-41.f, 8.f, -37.f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(0.f, 7.f, 0.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (6 == m_iVisitCount)
	{
		if (m_fTimer > -26.f)
		{
			// 전체 보면서 공전
			m_pCamera->m_pTransform->RotateAround(_vec3(0.f, 7.f, 0.f), _vec3(0.f, 1.f, 0.f), -0.15f * fTimeDelta);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(0.f, 7.f, 0.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));
		}
		else
		{
			// 위로 올려다 보면서 끝나고 엔딩
			m_pCamera->m_pTransform->m_vInfo[INFO_POS] = _vec3(-30.f, 5.f, -37.5f);
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &(_vec3(11.5f, 3.f, -22.f) - m_pCamera->m_pTransform->m_vInfo[INFO_POS]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT], D3DXVec3Cross(&_vec3(), &_vec3(0.f, 1.f, 0.f), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK]));
			D3DXVec3Normalize(&m_pCamera->m_pTransform->m_vInfo[INFO_UP], D3DXVec3Cross(&_vec3(), &m_pCamera->m_pTransform->m_vInfo[INFO_LOOK], &m_pCamera->m_pTransform->m_vInfo[INFO_RIGHT]));

			++m_iVisitCount;
		}
	}
	else if (7 == m_iVisitCount)
	{
		if (m_fTimer > - 55.f) //-210
		{
			CCameraManager::GetInstance()->LookAtTarget(_vec3(11.5f, 100.f, -22.f), 0.001f * fTimeDelta);
		}
		else
		{
			Engine::CGameObject* pGameObject = CBlackOutIn::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
			dynamic_cast<CTempUI*>(pGameObject)->Set_UIObjID(UIOBJECTTTAG::UIID_BASIC, 10);
			Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_BLACK, Engine::UILAYER::UI_UP, pGameObject);

			++m_iVisitCount;
		}
	}
	else if (8 == m_iVisitCount)
	{
	    CGameObject* pBlackOut = Engine::UIManager()->Get_PopupObject(POPUP_BLACK, UI_UP, UIID_BASIC, 10);
	    if (pBlackOut != nullptr && dynamic_cast<CBlackOutIn*>(pBlackOut)->Get_Middle()) {
	    	// 크레딧 UI 생성
			Engine::CGameObject* pGameObject = CUICredit::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
	    	Engine::UIManager()->Add_PopupGameobject(Engine::UIPOPUPLAYER::POPUP_BLACK, Engine::UILAYER::UI_DOWN, pGameObject);
	    
			m_iVisitCount = 0;

			m_fTimer = 10.f;
			m_eCurr_PD = PD::Normal;
			m_ePrev_PD = PD::Normal;

			CInputDev::GetInstance()->Lock_Input(false);
	    	return;
	    }
	}

	m_fTimer -= fTimeDelta;
}

void CGameManager::Free()
{
}
