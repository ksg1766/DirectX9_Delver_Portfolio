#include "Alchemist_Idle.h"
#include "Export_Function.h"
//플레이어나 인벱토리, 기타 아이템들 인클루드 할 예정

CNpc_Alchemist_Idle::CNpc_Alchemist_Idle()
{
}

CNpc_Alchemist_Idle::CNpc_Alchemist_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CNpc_Alchemist_Idle::~CNpc_Alchemist_Idle()
{
}

HRESULT CNpc_Alchemist_Idle::Ready_State(CStateMachine* pOwner)
{
	return S_OK;
}

STATE CNpc_Alchemist_Idle::Update_State(const _float& fTimeDelta)
{
	return STATE::IDLE;
}

void CNpc_Alchemist_Idle::LateUpdate_State()
{
}

void CNpc_Alchemist_Idle::Render_State()
{
}

CNpc_Alchemist_Idle* CNpc_Alchemist_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CNpc_Alchemist_Idle* pState = new CNpc_Alchemist_Idle(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CNpc_Alchemist_Idle*>(pState);

		MSG_BOX("Trader State Idle Failed");
		return nullptr;
	}

	return pState;
}

void CNpc_Alchemist_Idle::Free()
{
	__super::Free();
}
