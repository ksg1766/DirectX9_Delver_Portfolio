#include "Dog_Stand.h"
#include "Export_Function.h"
//플레이어나 인벱토리, 기타 아이템들 인클루드 할 예정

CNpc_Dog_Stand::CNpc_Dog_Stand()
{
}

CNpc_Dog_Stand::CNpc_Dog_Stand(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CNpc_Dog_Stand::~CNpc_Dog_Stand()
{
}

HRESULT CNpc_Dog_Stand::Ready_State(CStateMachine* pOwner)
{
	return S_OK;
}

STATE CNpc_Dog_Stand::Update_State(const _float& fTimeDelta)
{
	return STATE::IDLE;
}

void CNpc_Dog_Stand::LateUpdate_State()
{
}

void CNpc_Dog_Stand::Render_State()
{
}

CNpc_Dog_Stand* CNpc_Dog_Stand::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CNpc_Dog_Stand* pState = new CNpc_Dog_Stand(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CNpc_Dog_Stand*>(pState);

		MSG_BOX("Trader State Idle Failed");
		return nullptr;
	}

	return pState;
}

void CNpc_Dog_Stand::Free()
{
	__super::Free();
}
