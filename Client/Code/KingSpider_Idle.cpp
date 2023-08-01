#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_Idle.h"
#include "Export_Function.h"
#include "Player.h"
#include "KingSpider.h"
#include "FlyingCamera.h"
#include "CameraManager.h"
#include "KingSpiderWeb.h"
#include "KingSpiderFog.h"
#include "KingSpiderPoison.h"
#include "KingSpiderNest.h"
#include "KingSpider_Jump.h"
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
	m_fJumpAttack = false;
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
	if(3.f < m_fDelay)
	{
		m_fDelay = 0.f;
		//Shoot_Web();
		Shoot_Poison();
	}
	/*if (10.f < m_fDelay)
	{
		m_fDelay = 0.f;
		if (BOSSPHASE::PHASE1 == dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_Phase())
		{
			switch (m_iSkillCount)
			{
			case 0:
				++m_iSkillCount;
				Shoot_Web();
				break;

			case 1:
				++m_iSkillCount;
				Shoot_Poison();
				break;

			case 2:
				m_iSkillCount = 0;
				m_fJumpAttack = false;
				Earthquake();
				break;
			}
		}
	}*/

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

void CKingSpider_Idle::Shoot_Web()
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CKingSpiderWeb::Create(m_pGraphicDev);
	dynamic_cast<CKingSpiderWeb*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	dynamic_cast<CKingSpiderWeb*>(pGameObject)->Set_Dir(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS]);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
}

void CKingSpider_Idle::Shoot_Poison()
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CKingSpiderPoison::Create(m_pGraphicDev);
	dynamic_cast<CKingSpiderPoison*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	dynamic_cast<CKingSpiderPoison*>(pGameObject)->Set_Dir(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS]);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
}

void CKingSpider_Idle::Earthquake()
{
	if (!m_fJumpAttack)
	{
		if (15.f >= m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y)
		{
			CFlyingCamera* pCamera = dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());
			pCamera->Set_ShakeForce(0.f, 0.5f, 1.5f, 2.f);
			pCamera->Shake_Camera();

			dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(false);
			m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y = 15.f;
			
			for (int i = 0; i < 7; ++i)
			{
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CKingSpiderFog::Create(m_pGraphicDev);
				dynamic_cast<CKingSpiderFog*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_vFogPos[i];
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}

			m_fJumpAttack = true;
			if (15 >= SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS].y)
			{
				CPlayerStat& PlayerState = *(dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer())->Get_Stat());
				PlayerState.Take_Damage(3.f);
			}
		}
		dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->Add_Force(_vec3(0.f, 20.f, 0.f));
		dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(true);
	}
}

void CKingSpider_Idle::Make_Nest()
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CKingSpiderNest::Create(m_pGraphicDev);
	dynamic_cast<CKingSpiderNest*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_LOOK].x * 2.f, 15.f, m_pOwner->Get_Transform()->m_vInfo[INFO_LOOK].z * 2.f);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
}

void CKingSpider_Idle::Free()
{
	__super::Free();
}
