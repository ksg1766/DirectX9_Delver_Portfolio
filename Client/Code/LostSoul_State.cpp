#include "LostSoul_State.h"
#include "Export_Function.h"
#include "Boss_LostSoul.h"
CLostSoul_State::CLostSoul_State()
{
}

CLostSoul_State::CLostSoul_State(LPDIRECT3DDEVICE9 pGrapicDev)
    :CState(pGrapicDev)
{
}

CLostSoul_State::~CLostSoul_State()
{
}

HRESULT CLostSoul_State::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    return S_OK;
}

STATE CLostSoul_State::Update_State(const _float& fTimeDelta)
{
    return STATE::IDLE;
}

void CLostSoul_State::LateUpdate_State()
{
}

void CLostSoul_State::Render_State()
{
}

CLostSoul_State* CLostSoul_State::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CLostSoul_State* pState = new CLostSoul_State(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CLostSoul_State*>(pState);

		MSG_BOX("BossSleep State Failed");
	}
	return pState;
}

void CLostSoul_State::Free()
{
	__super::Free();
}
