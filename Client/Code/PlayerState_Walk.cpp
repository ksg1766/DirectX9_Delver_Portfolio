#include "stdafx.h"
#include "SoundManager.h"
#include "..\Header\PlayerState_Walk.h"
#include "Export_Function.h"
#include "DynamicCamera.h"
#include "Player.h"
#include "Beer.h"

CPlayerState_Walk::CPlayerState_Walk()
{
}

CPlayerState_Walk::CPlayerState_Walk(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CState(pGraphicDev)
{

}

CPlayerState_Walk::~CPlayerState_Walk()
{
}

HRESULT CPlayerState_Walk::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;

	return S_OK;
}

STATE CPlayerState_Walk::Update_State(const _float& fTimeDelta)
{

	STATE eState = STATE::IDLE;

	if (!static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_UseUI())
		eState = Key_Input(fTimeDelta);

	return eState;
}

void CPlayerState_Walk::LateUpdate_State()
{
}

void CPlayerState_Walk::Render_State()
{
	//cout << "Walk" << endl;
}

STATE CPlayerState_Walk::Key_Input(const _float& fTimeDelta)
{
	_vec3 vLook = m_pOwner->Get_Transform()->m_vInfo[INFO_LOOK];
	_vec3 vRight = m_pOwner->Get_Transform()->m_vInfo[INFO_RIGHT];

	CPlayer& pPlayer = *static_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());
	STATE	_eState = STATE::IDLE;


	if (!pPlayer.IsTalk())
	{
		if (Engine::InputDev()->Key_Pressing(DIK_W))
		{
			_vec3 vFront = vLook;
			vFront.y = 0.f;
			if(static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_Slow())
				m_pOwner->Get_Transform()->Translate(2.f * fTimeDelta * vFront);
			else if (!static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_Slow())
				m_pOwner->Get_Transform()->Translate(6.f * fTimeDelta * vFront);
			_eState = STATE::ROMIMG;
		}
		if (Engine::InputDev()->Key_Pressing(DIK_S))
		{
			_vec3 vFront = vLook;
			vFront.y = 0.f;
			if (static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_Slow())
				m_pOwner->Get_Transform()->Translate(2.f * fTimeDelta * -vFront);
			else if(!static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_Slow())
				m_pOwner->Get_Transform()->Translate(6.f * fTimeDelta * -vFront);
			_eState = STATE::ROMIMG;
		}
		if (Engine::InputDev()->Key_Pressing(DIK_A))
		{
			_vec3 vSide = vRight;
			vSide.y = 0.f;
			if (static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_Slow())
				m_pOwner->Get_Transform()->Translate(2.f * fTimeDelta * -vSide);
			else if (!static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_Slow())
				m_pOwner->Get_Transform()->Translate(6.f * fTimeDelta * -vSide);
			_eState = STATE::ROMIMG;
		}
		if (Engine::InputDev()->Key_Pressing(DIK_D))
		{
			_vec3 vSide = vRight;
			vSide.y = 0.f;
			if (static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_Slow())
				m_pOwner->Get_Transform()->Translate(2.f * fTimeDelta * vSide);
			else if (!static_cast<CPlayer*>(m_pOwner->Get_Host())->Get_Slow())
				m_pOwner->Get_Transform()->Translate(6.f * fTimeDelta * vSide);
			_eState = STATE::ROMIMG;
		}

		if (pPlayer.Get_CurrentEquipRight())
			if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
			{
				static_cast<CPlayer*>(m_pOwner->Get_Host())->Set_AttackTick(true);
				_eState = STATE::ATTACK;
			}

		if (Engine::InputDev()->Mouse_Down(DIM_RB))
		{
			// TODO : 마우스 오른 쪽 누르면 소모류는 사용 가능하게.
			// 스테이트 반영은 필요X

			CPlayer& pPlayer = *static_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

			if (pPlayer.Get_CurrentEquipLeft() == nullptr)
				return STATE::IDLE;

			// 왼쪽 손에 맥주를 장착하고 있다면
			if (static_cast<CItem*>(pPlayer.Get_CurrentEquipLeft())->Get_ItemTag().eItemID == ITEMID::GENERAL_BEER)
				if (static_cast<CBeer*>(pPlayer.Get_CurrentEquipLeft())->Get_BeerCount() >= 1.f)
					static_cast<CBeer*>(pPlayer.Get_CurrentEquipLeft())->Use_Beer(1.f);

			if (static_cast<CItem*>(pPlayer.Get_CurrentEquipLeft())->Get_ItemTag().eItemID == ITEMID::GENERAL_SHIELD)
			{
				pPlayer.Set_ThrowShield(true);
				return STATE::ATTACK;
			}

		}

		return _eState;
	}
	
	return _eState;
}

CPlayerState_Walk* CPlayerState_Walk::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CPlayerState_Walk* pState = new CPlayerState_Walk(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CPlayerState_Walk*>(pState);

		MSG_BOX("Create WalkState Failed");
		return nullptr;
	}

	return pState;
}

void CPlayerState_Walk::Free()
{
	__super::Free();
}
