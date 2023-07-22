#include "GrapPattern.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"

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




	return STATE::BOSS_IDLE;
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
