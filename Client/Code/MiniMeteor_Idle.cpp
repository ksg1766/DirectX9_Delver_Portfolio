#include "MiniMeteor_Idle.h"
#include "Export_Function.h"

CMiniMeteor_Idle::CMiniMeteor_Idle()
{
}

CMiniMeteor_Idle::CMiniMeteor_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CMiniMeteor_Idle::~CMiniMeteor_Idle()
{
}

HRESULT CMiniMeteor_Idle::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	return S_OK;
}

STATE CMiniMeteor_Idle::Update_State(const _float& fTimeDelta)
{  
	return STATE::IDLE;
}

void CMiniMeteor_Idle::LateUpdate_State()
{
}

void CMiniMeteor_Idle::Render_State()
{
}

CMiniMeteor_Idle* CMiniMeteor_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMiniMeteor_Idle* pState = new CMiniMeteor_Idle(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMiniMeteor_Idle*>(pState);

		MSG_BOX("Boss Attack State Failed");
	}
	return pState;
}

void CMiniMeteor_Idle::Free()
{
	__super::Free();
}
