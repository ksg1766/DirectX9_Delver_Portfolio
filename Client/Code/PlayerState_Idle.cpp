#include "..\Header\PlayerState_Idle.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "Player.h"

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
	STATE eState = STATE::IDLE;

	if (!dynamic_cast<CPlayer*>(m_pOwner->Get_Host())->Get_UseUI())
		eState = Key_Input(fTimeDelta);

	return eState;
}

void CPlayerState_Idle::LateUpdate_State()
{
}

void CPlayerState_Idle::Render_State()
{
	//cout << "IDLE" << endl;
}

STATE CPlayerState_Idle::Key_Input(const _float& fTimeDelta)
{
	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (Engine::InputDev()->GetInstance()->Key_Down(DIK_W))
		return STATE::ROMIMG;
	if (Engine::InputDev()->GetInstance()->Key_Down(DIK_S))
		return STATE::ROMIMG;
	if (Engine::InputDev()->GetInstance()->Key_Down(DIK_A))
		return STATE::ROMIMG;
	if (Engine::InputDev()->GetInstance()->Key_Down(DIK_D))
		return STATE::ROMIMG;
	
	if (Engine::InputDev()->GetInstance()->Mouse_Pressing(DIM_LB))
	{
		dynamic_cast<CPlayer*>(m_pOwner->Get_Host())->Set_AttackTick(true);
		return STATE::ATTACK;
	}


	
	pPlayer.Set_State(STATE::IDLE);
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
