#include "Dog_Idle.h"
#include "Export_Function.h"
//플레이어나 인벱토리, 기타 아이템들 인클루드 할 예정

CNpc_Dog_Idle::CNpc_Dog_Idle()
{
}

CNpc_Dog_Idle::CNpc_Dog_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CNpc_Dog_Idle::~CNpc_Dog_Idle()
{
}

HRESULT CNpc_Dog_Idle::Ready_State(CStateMachine* pOwner)
{
	return S_OK;
}

STATE CNpc_Dog_Idle::Update_State(const _float& fTimeDelta)
{
	return STATE::IDLE;
}

void CNpc_Dog_Idle::LateUpdate_State()
{
}

void CNpc_Dog_Idle::Render_State()
{
}

CNpc_Dog_Idle* CNpc_Dog_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CNpc_Dog_Idle* pState = new CNpc_Dog_Idle(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CNpc_Dog_Idle*>(pState);

		MSG_BOX("Trader State Idle Failed");
		return nullptr;
	}

	return pState;
}

void CNpc_Dog_Idle::Free()
{
	__super::Free();
}
