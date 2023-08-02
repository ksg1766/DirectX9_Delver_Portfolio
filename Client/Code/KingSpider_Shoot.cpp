#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_Shoot.h"
#include "Export_Function.h"
#include "Player.h"
#include "KingSpiderWeb.h"
CKingSpider_Shoot::CKingSpider_Shoot()
{
}

CKingSpider_Shoot::CKingSpider_Shoot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_Shoot::~CKingSpider_Shoot()
{
}

HRESULT CKingSpider_Shoot::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_iSkillCount = 0.f;


	return S_OK;
}

STATE CKingSpider_Shoot::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;
	if (1.5f < m_fDelay)
	{
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CKingSpiderWeb::Create(m_pGraphicDev);
		dynamic_cast<CKingSpiderWeb*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		dynamic_cast<CKingSpiderWeb*>(pGameObject)->Set_Dir(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS]);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_fDelay = 0.f;
		++m_iSkillCount;
	}

	if (2 < m_iSkillCount)
	{
		m_fDelay = 0.f;
		m_iSkillCount = 0.f;
		return STATE::BOSS_IDLE;
	}
}

void CKingSpider_Shoot::LateUpdate_State()
{
}

void CKingSpider_Shoot::Render_State()
{
}

CKingSpider_Shoot* CKingSpider_Shoot::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_Shoot* pState = new CKingSpider_Shoot(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_Shoot*>(pState);

		MSG_BOX("KingSpider_Shoot Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_Shoot::Free()
{
	__super::Free();
}
