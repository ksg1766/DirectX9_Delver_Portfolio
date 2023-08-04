#include "stdafx.h"
#include "SoundManager.h"
#include "KingSpider_ShootPoison.h"
#include "Export_Function.h"
#include "Player.h"
#include "KingSpiderPoison.h"
#include "KingSpider.h"
CKingSpider_ShootPoison::CKingSpider_ShootPoison()
{
}

CKingSpider_ShootPoison::CKingSpider_ShootPoison(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CKingSpider_ShootPoison::~CKingSpider_ShootPoison()
{
}

HRESULT CKingSpider_ShootPoison::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_iSkillCount = 0;
	return S_OK;
}

STATE CKingSpider_ShootPoison::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;
		Engine::CGameObject* pGameObject = nullptr;

			if (0.5f < m_fDelay)
			{
				CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BOSS);
				CSoundManager::GetInstance()->PlaySound(L"KingSpider_Attack2.wav", CHANNELID::SOUND_BOSS, 1.f);

				pGameObject = CKingSpiderPoison::Create(m_pGraphicDev);
				static_cast<CKingSpiderPoison*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
				static_cast<CKingSpiderPoison*>(pGameObject)->Set_DirParabola(SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS]);
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
				++m_iSkillCount;
				m_fDelay = 0.f;
			}
			if (5 <= m_iSkillCount)
			{
				m_fDelay = 0.f;
				m_iSkillCount = 0.f;
				return STATE::BOSS_IDLE;
			}
}

void CKingSpider_ShootPoison::LateUpdate_State()
{
}

void CKingSpider_ShootPoison::Render_State()
{
}

CKingSpider_ShootPoison* CKingSpider_ShootPoison::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CKingSpider_ShootPoison* pState = new CKingSpider_ShootPoison(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CKingSpider_ShootPoison*>(pState);

		MSG_BOX("KingSpider_Jump Failed");
		return nullptr;
	}

	return pState;
}

void CKingSpider_ShootPoison::Free()
{
	__super::Free();
}
