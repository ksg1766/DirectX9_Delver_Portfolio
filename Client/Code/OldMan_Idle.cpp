#include "OldMan_Idle.h"
#include "Export_Function.h"
//플레이어나 인벱토리, 기타 아이템들 인클루드 할 예정

CNpc_OldMan_Idle::CNpc_OldMan_Idle()
{
}

CNpc_OldMan_Idle::CNpc_OldMan_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CNpc_OldMan_Idle::~CNpc_OldMan_Idle()
{
}

HRESULT CNpc_OldMan_Idle::Ready_State(CStateMachine* pOwner)
{
	return S_OK;
}

STATE CNpc_OldMan_Idle::Update_State(const _float& fTimeDelta)
{
	return STATE::IDLE;
}

void CNpc_OldMan_Idle::LateUpdate_State()
{
}

void CNpc_OldMan_Idle::Render_State()
{
}

CNpc_OldMan_Idle* CNpc_OldMan_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CNpc_OldMan_Idle* pState = new CNpc_OldMan_Idle(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CNpc_OldMan_Idle*>(pState);

		MSG_BOX("Trader State Idle Failed");
		return nullptr;
	}

	return pState;
}

void CNpc_OldMan_Idle::Free()
{
	__super::Free();
}
