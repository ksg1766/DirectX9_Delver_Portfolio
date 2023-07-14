#include "Bard_Idle.h"
#include "Export_Function.h"
//플레이어나 인벱토리, 기타 아이템들 인클루드 할 예정

CNpc_Bard_Idle::CNpc_Bard_Idle()
{
}

CNpc_Bard_Idle::CNpc_Bard_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CNpc_Bard_Idle::~CNpc_Bard_Idle()
{
}

HRESULT CNpc_Bard_Idle::Ready_State(CStateMachine* pOwner)
{
	return S_OK;
}

STATE CNpc_Bard_Idle::Update_State(const _float& fTimeDelta)
{
	return STATE::IDLE;
}

void CNpc_Bard_Idle::LateUpdate_State()
{
}

void CNpc_Bard_Idle::Render_State()
{
}

CNpc_Bard_Idle* CNpc_Bard_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CNpc_Bard_Idle* pState = new CNpc_Bard_Idle(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CNpc_Bard_Idle*>(pState);

		MSG_BOX("Trader State Idle Failed");
		return nullptr;
	}

	return pState;
}

void CNpc_Bard_Idle::Free()
{
	__super::Free();
}
