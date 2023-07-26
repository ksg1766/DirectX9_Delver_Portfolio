#include "LostSoulPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_LostSoul.h"
#include "Player.h"
CLostSoulPattern::CLostSoulPattern()
{
}

CLostSoulPattern::CLostSoulPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CLostSoulPattern::~CLostSoulPattern()
{
}

HRESULT CLostSoulPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_iSkillCount = 0;

	return S_OK;
}

STATE CLostSoulPattern::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if (2.5f < m_fDelay)
	{
		pGameObject = CBossLostSoul::Create(m_pGraphicDev);
		dynamic_cast<CBossLostSoul*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		dynamic_cast<CBossLostSoul*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

		m_fDelay = 0.f;
	++m_iSkillCount;
	}

	if (3 <= m_iSkillCount)
	{
		m_iSkillCount = 0.f;
		return STATE::BOSS_IDLE;
	}
}

void CLostSoulPattern::LateUpdate_State()
{
}

void CLostSoulPattern::Render_State()
{
}

CLostSoulPattern* CLostSoulPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CLostSoulPattern* pState = new CLostSoulPattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CLostSoulPattern*>(pState);

		MSG_BOX("LostSoul State Failed");
	}
	return pState;
}

void CLostSoulPattern::Free()
{
	__super::Free();
}
