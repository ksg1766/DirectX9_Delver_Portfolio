#include "Blade_Trap_Idle.h"
#include "Export_Function.h"
#include "Blade_Trap_Body.h"
#include "Player.h"

CBlade_Trap_Idle::CBlade_Trap_Idle()
{
}

CBlade_Trap_Idle::CBlade_Trap_Idle(LPDIRECT3DDEVICE9 pGrapicDev)
	:CState(pGrapicDev)
{
}

CBlade_Trap_Idle::~CBlade_Trap_Idle()
{
}

HRESULT CBlade_Trap_Idle::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	return S_OK;
}

STATE CBlade_Trap_Idle::Update_State(const _float& fTimeDelta)
{
	_vec3 vPlayerPos = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
	_vec3 vDir = vPlayerPos - m_vTrapCenter;

	_float fDistance = D3DXVec3LengthSq(&vDir);
	if (fDistance < pow(4, 2))
	{
		return STATE::ATTACK;
	}
	else
	{
		m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y = m_vTrapCenter.y -1.f;
		return STATE::IDLE;
	}
}

void CBlade_Trap_Idle::LateUpdate_State()
{
}

void CBlade_Trap_Idle::Render_State()
{
	//테스트 상태로그 확인용 출력(cout)
}

CBlade_Trap_Idle* CBlade_Trap_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBlade_Trap_Idle* pState = new CBlade_Trap_Idle(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBlade_Trap_Idle*>(pState);

		MSG_BOX("Blade_Trap_Idle State Failed");
	}
	return pState;
}

void CBlade_Trap_Idle::Free()
{
	__super::Free();
}
