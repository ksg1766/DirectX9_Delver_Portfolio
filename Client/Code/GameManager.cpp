#include "stdafx.h"
#include "..\Header\GameManager.h"
#include "SoundManager.h"
#include "Player.h"
#include "SkeletonKing.h"
#include "CameraManager.h"
#include "FlyingCamera.h"
//#include "KingSpider.h"
#include "EffectSwordTrail.h"
#include "EffectSwordLightning.h"
#include "EffectSwordParticles.h"

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

	switch (m_eCurr_PD)
	{
	case PD::Normal:
		break;
	case PD::ShowVillage:
		break;
	case PD::ShowSewer:
		break;
	case PD::ShowMiniBoss:
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

void CGameManager::ShowMiniBoss(const _float& fTimeDelta)
{
	CGameObject* pBoss = SceneManager()->Get_Scene()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front();
	
	if (m_fTimer > 9.f)
	{
		m_fTimer -= fTimeDelta;
		CCameraManager::GetInstance()->LookAtTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta);
	}
	else if (m_fTimer > 8.f)
	{
		m_fTimer -= fTimeDelta;
		CCameraManager::GetInstance()->ZoomInTarget(pBoss->m_pTransform->m_vInfo[INFO_POS], fTimeDelta, 3.f);
	}
	else if (m_fTimer > 7.f)
	{
		m_fTimer -= fTimeDelta;
		CCameraManager::GetInstance()->ZoomOutToTrans(m_pPlayer->m_pTransform, fTimeDelta);
	}
	else
	{
		m_pCamera->m_pTransform->Copy_RUL_AddPos(m_pPlayer->m_pTransform->m_vInfo);
		m_fTimer = 10.f;
		m_eCurr_PD = PD::Normal;
		m_ePrev_PD = PD::Normal;
	}
}

void CGameManager::HekirekiIssen(const _float& fTimeDelta)	// 일단 절차지향으로 구현해보자
{
	if (PD::Normal == m_ePrev_PD && !m_bReadyBreath)
	{
		m_bReadyBreath = true;
		m_fTimer = 15.5f;
		SceneManager()->SlowMode(m_fTimer, 5);
		m_ePrev_PD = m_eCurr_PD;

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
		else if (m_fTimer > -1.f)
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
		}
	}
}

void CGameManager::Free()
{
}
