#include "..\Header\PlayerState_Idle.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "Beer.h"
#include "DynamicCamera.h"

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
	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (Engine::InputDev()->Key_Down(DIK_W))
		return STATE::ROMIMG;
	if (Engine::InputDev()->Key_Down(DIK_S))
		return STATE::ROMIMG;
	if (Engine::InputDev()->Key_Down(DIK_A))
		return STATE::ROMIMG;
	if (Engine::InputDev()->Key_Down(DIK_D))
		return STATE::ROMIMG;
	
	if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
	{
		dynamic_cast<CPlayer*>(m_pOwner->Get_Host())->Set_AttackTick(true);
		return STATE::ATTACK;
	}

	if (Engine::InputDev()->Mouse_Down(DIM_RB))
	{
		// TODO : 마우스 오른 쪽 누르면 소모류는 사용 가능하게.
		// 스테이트 반영은 필요X

		CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

		if (pPlayer.Get_CurrentEquipLeft() == nullptr)
			return STATE::IDLE;

			// 왼쪽 손에 맥주를 장착하고 있다면
		if (dynamic_cast<CItem*>(pPlayer.Get_CurrentEquipLeft())->Get_ItemTag().eItemID == ITEMID::GENERAL_BEER)
		{
			if (dynamic_cast<CBeer*>(pPlayer.Get_CurrentEquipLeft())->Get_BeerCount() >= 1.f)
			{
				dynamic_cast<CBeer*>(pPlayer.Get_CurrentEquipLeft())->Use_Beer(1.f);

				pPlayer.Set_Drunk(true);
			}
		}
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
