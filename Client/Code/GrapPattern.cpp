#include "GrapPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"
#include "Boss_GrabBat.h"
CGrapPattern::CGrapPattern()
{
}

CGrapPattern::CGrapPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CGrapPattern::~CGrapPattern()
{
}

HRESULT CGrapPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_bCool = false;
	return S_OK;
}

STATE CGrapPattern::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if (5.f < m_fDelay)
	{
		pGameObject = CBoss_GrabBat::Create(m_pGraphicDev);
		dynamic_cast<CBoss_GrabBat*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_fDelay = 0.f;
	}

	return STATE::BOSS_PH2SKILL4;
}

void CGrapPattern::LateUpdate_State()
{
}

void CGrapPattern::Render_State()
{
}

CGrapPattern* CGrapPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CGrapPattern* pState = new CGrapPattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CGrapPattern*>(pState);

		MSG_BOX("Fire State Failed");
	}
	return pState;
}

void CGrapPattern::Free()
{
	__super::Free();
}
