#include "..\Header\PlayerState_Idle.h"
#include "Export_Function.h"

CPlayerState_Idle::CPlayerState_Idle()
{
}

CPlayerState_Idle::CPlayerState_Idle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CState(pGraphicDev)
{
}

CPlayerState_Idle::~CPlayerState_Idle()
{
}

HRESULT CPlayerState_Idle::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;

	return S_OK;
}

STATE CPlayerState_Idle::Update_State(const _float& fTimeDelta)
{
	STATE eState = Key_Input(fTimeDelta);

	return eState;
}

void CPlayerState_Idle::LateUpdate_State()
{
}

void CPlayerState_Idle::Render_State()
{
	cout << "IDLE" << endl;
}

STATE CPlayerState_Idle::Key_Input(const _float& fTimeDelta)
{
	if (Engine::InputDev()->GetInstance()->Key_Down(DIK_W))
		return STATE::ROMIMG;
	if (Engine::InputDev()->GetInstance()->Key_Down(DIK_S))
		return STATE::ROMIMG;
	if (Engine::InputDev()->GetInstance()->Key_Down(DIK_A))
		return STATE::ROMIMG;
	if (Engine::InputDev()->GetInstance()->Key_Down(DIK_D))
		return STATE::ROMIMG;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X))
		m_pOwner->Get_Transform()->Rotate(ROT_Y, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

	if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y))
		m_pOwner->Get_Transform()->Rotate(ROT_X, D3DXToRadian(dwMouseMove) * fTimeDelta * 3.f);

	if (Engine::InputDev()->GetInstance()->Get_DIMouseState(DIM_LB))
		return STATE::ATTACK;

	return STATE::IDLE;
}

CPlayerState_Idle* CPlayerState_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CPlayerState_Idle* pState = new CPlayerState_Idle(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CPlayerState_Idle*>(pState);

		MSG_BOX("Create PlayerIdle Failed");
		return nullptr;
	}

	return pState;
}

void CPlayerState_Idle::Free()
{
	__super::Free();
}
