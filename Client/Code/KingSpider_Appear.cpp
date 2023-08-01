#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_Appear.h"
#include "Export_Function.h"
#include "Player.h"
#include "BookDoor.h"
#include "KingSpider.h"
#include "FlyingCamera.h"
#include "CameraManager.h"
#include "GameManager.h"

CKingSpider_Appear::CKingSpider_Appear()
{
}

CKingSpider_Appear::CKingSpider_Appear(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_Appear::~CKingSpider_Appear()
{
}

HRESULT CKingSpider_Appear::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_bAppearTrigger = false;
	m_bJumpTrigger = false;
	m_bRending = false;
	return S_OK;
}

STATE CKingSpider_Appear::Update_State(const _float& fTimeDelta)
{
	if (!m_bAppearTrigger)
	{
		CGameObject* pGameObject = nullptr;
		pGameObject = dynamic_cast<CBookDoor*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::TRIGERBLOCK).back());
		m_bAppearTrigger = dynamic_cast<CBookDoor*>(pGameObject)->Get_bTrigger();
	}

	if (m_bAppearTrigger && !m_bJumpTrigger && 0.f == m_fDelay)
 		CGameManager::GetInstance()->PlayMode(PD::ShowMiniBoss);

	if (m_bAppearTrigger)
		m_fDelay += fTimeDelta;

	if ((4.5f < m_fDelay) && (!m_bJumpTrigger))
	{
		m_vDir = _vec3(-42.f, 12.f, -18.18f) - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->Add_Force(_vec3(m_vDir.x * 0.175f, 15.f, m_vDir.z * 0.175f));
		dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(true);

		m_bJumpTrigger = true;
		m_fDelay = 0.f;
	}
	if ((!m_bRending) && (15.5f >= m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y))
	{
		dynamic_cast<CKingSpider*>(m_pOwner->Get_Host())->Get_RigidBody()->UseGravity(false);
		m_fDelay = 0.f;

		CFlyingCamera* pCamera = dynamic_cast<CFlyingCamera*>(CCameraManager::GetInstance()->Get_CurrentCam());
		pCamera->Set_ShakeForce(0.f, 0.5f, 1.5f, 2.f);
		pCamera->Shake_Camera();

		m_bRending = true;
	}
	if ((3.f < m_fDelay) && m_bRending)
	{
		return STATE::BOSS_IDLE;
	}
}

void CKingSpider_Appear::LateUpdate_State()
{
}

void CKingSpider_Appear::Render_State()
{
}

CKingSpider_Appear* CKingSpider_Appear::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_Appear* pState = new CKingSpider_Appear(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_Appear*>(pState);

		MSG_BOX("KingSpider_Appear Failed");
		return nullptr;
	}
	return pState;
}

void CKingSpider_Appear::Free()
{
	__super::Free();
}
