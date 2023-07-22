#include "CrossPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"

CCrossPattern::CCrossPattern()
{
}

CCrossPattern::CCrossPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CCrossPattern::~CCrossPattern()
{
}

HRESULT CCrossPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_bCool = false;
	return S_OK;
}

STATE CCrossPattern::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;




	return STATE::BOSS_IDLE;
}

void CCrossPattern::LateUpdate_State()
{
}

void CCrossPattern::Render_State()
{
}

CCrossPattern* CCrossPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CCrossPattern* pState = new CCrossPattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CCrossPattern*>(pState);

		MSG_BOX("Fire State Failed");
	}
	return pState;
}

void CCrossPattern::Free()
{
	__super::Free();
}
