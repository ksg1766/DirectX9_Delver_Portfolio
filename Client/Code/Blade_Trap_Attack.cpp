#include "Blade_Trap_Attack.h"
#include "Export_Function.h"

CBlade_Trap_Attack::CBlade_Trap_Attack()
{
}

CBlade_Trap_Attack::CBlade_Trap_Attack(LPDIRECT3DDEVICE9 pGrapicDev)
	:CState(pGrapicDev)
{
}

CBlade_Trap_Attack::~CBlade_Trap_Attack()
{
}

HRESULT CBlade_Trap_Attack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bAttack = false;
	m_fCool = false;
	return S_OK;
}

STATE CBlade_Trap_Attack::Update_State(const _float& fTimeDelta)
{
	//해당 상태일 때 업데이트 할 것들
	if (m_bAttack)
		m_fCool += fTimeDelta;
	if ((3.f < m_fCool) && (m_bAttack))
	{

	}
	return STATE::IDLE;
}

void CBlade_Trap_Attack::LateUpdate_State()
{

}

void CBlade_Trap_Attack::Render_State()
{
	//테스트 상태로그 확인용 출력(cout)
}

CBlade_Trap_Attack* CBlade_Trap_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBlade_Trap_Attack* pState = new CBlade_Trap_Attack(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBlade_Trap_Attack*>(pState);

		MSG_BOX("Blade_Trap_Idle State Failed");
	}
	return pState;
}

void CBlade_Trap_Attack::Free()
{
	__super::Free();
}
