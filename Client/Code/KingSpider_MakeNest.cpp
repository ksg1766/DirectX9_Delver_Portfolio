#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_MakeNest.h"
#include "Export_Function.h"
#include "Player.h"
#include "KingSpiderNest.h"
#include "DungeonSpider.h"
CKingSpider_MakeNest::CKingSpider_MakeNest()
{
}

CKingSpider_MakeNest::CKingSpider_MakeNest(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_MakeNest::~CKingSpider_MakeNest()
{
}

HRESULT CKingSpider_MakeNest::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_bMakeNest = false;
	m_vSpawnPos[0] = _vec3(-6.5f, 0.f, -6.5f);
	m_vSpawnPos[1] = _vec3(6.5f, 0.f, -6.5f);
	m_vSpawnPos[2] = _vec3(-6.5f, 0.f, 6.5f);
	m_vSpawnPos[3] = _vec3(6.5f, 0.f, 6.5f);
	return S_OK;
}

STATE CKingSpider_MakeNest::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;
	if (!m_bMakeNest)
	{

		Engine::CGameObject* pGameObject = nullptr;
		for (int i = 0; i < 4; ++i)
		{
			pGameObject = CDungeonSpider::Create(m_pGraphicDev);
			static_cast<CDungeonSpider*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vSpawnPos[i];
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_bMakeNest = true;
	}
	if (6.f < m_fDelay)
	{
		m_bMakeNest = false;
		m_fDelay = 0.f;
		return STATE::BOSS_IDLE;
	}
}

void CKingSpider_MakeNest::LateUpdate_State()
{
}

void CKingSpider_MakeNest::Render_State()
{
}

CKingSpider_MakeNest* CKingSpider_MakeNest::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_MakeNest* pState = new CKingSpider_MakeNest(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_MakeNest*>(pState);

		MSG_BOX("KingSpider_Jump Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_MakeNest::Free()
{
	__super::Free();
}
