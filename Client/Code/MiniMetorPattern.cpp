#include "MiniMetorPattern.h"

CMiniMetorPattern::CMiniMetorPattern()
{
}

CMiniMetorPattern::CMiniMetorPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CMiniMetorPattern::~CMiniMetorPattern()
{
}

HRESULT CMiniMetorPattern::Ready_State(CStateMachine* pOwner)
{
	return S_OK;
}

STATE CMiniMetorPattern::Update_State(const _float& fTimeDelta)
{
	return STATE::BOSS_IDLE;
}

void CMiniMetorPattern::LateUpdate_State()
{
}

void CMiniMetorPattern::Render_State()
{
}

CMiniMetorPattern* CMiniMetorPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMiniMetorPattern* pState = new CMiniMetorPattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMiniMetorPattern*>(pState);

		MSG_BOX("MiniMeteor State Failed");
	}
	return pState;
}

void CMiniMetorPattern::Free()
{
	__super::Free();
}
