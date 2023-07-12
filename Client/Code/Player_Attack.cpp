#include "..\Header\Player_Attack.h"
#include "Export_Function.h"
#include "Player.h"
#include "Item.h"

CPlayer_Attack::CPlayer_Attack()
{
}

CPlayer_Attack::CPlayer_Attack(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CState(pGraphicDev)
{
}

CPlayer_Attack::~CPlayer_Attack()
{
}

HRESULT CPlayer_Attack::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;


	return S_OK;
}

STATE CPlayer_Attack::Update_State(const _float& fTimeDelta)
{
	STATE eState = Key_Input(fTimeDelta);

	return eState;
}

void CPlayer_Attack::LateUpdate_State()
{
}

void CPlayer_Attack::Render_State()
{
	//cout << "Attack" << endl;
}

STATE CPlayer_Attack::Key_Input(const _float& fTimeDelta)
{
	_vec3 vLook = m_pOwner->Get_Transform()->m_vInfo[INFO_LOOK];
	_vec3 vRight = m_pOwner->Get_Transform()->m_vInfo[INFO_RIGHT];

	STATE	_eState = STATE::ROMIMG;

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->GetInstance()
		->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (Engine::InputDev()->GetInstance()->Key_Pressing(DIK_W))
	{
		m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * vLook);
	}


	if (Engine::InputDev()->GetInstance()->Key_Pressing(DIK_S))
	{
		m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * -vLook);
	}

	if (Engine::InputDev()->GetInstance()->Key_Pressing(DIK_A))
	{
		m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * -vRight);
	}

	if (Engine::InputDev()->GetInstance()->Key_Pressing(DIK_D))
	{
		m_pOwner->Get_Transform()->Translate(10.f * fTimeDelta * vRight);
	}

	if (!pPlayer.Get_Attack())
	{
		if (pPlayer.Get_CurrentEquipRight() != nullptr)
		{
			ITEMTYPEID _eID = dynamic_cast<CItem*>(pPlayer.Get_CurrentEquipRight())->Get_ItemTag();

			switch (_eID.eItemID)
			{
			case ITEMID::WEAPON_SWORD:
				if (Engine::InputDev()->GetInstance()->Mouse_Pressing(DIM_LB))
				{
					pPlayer.Set_Attack(true);
					pPlayer.Set_AttackTick(false);
					pPlayer.Set_State(STATE::ATTACK);
					return STATE::ATTACK;
				}
				break;
			case ITEMID::WEAPON_BOW:
				if (Engine::InputDev()->GetInstance()->Mouse_Pressing(DIM_LB))
				{
					pPlayer.Set_Attack(true);
					pPlayer.Set_AttackTick(false);
					pPlayer.Set_State(STATE::ATTACK);
					return STATE::ATTACK;
				}
				else if (Engine::InputDev()->GetInstance()->Mouse_Up(DIM_LB))
				{
					

					return STATE::ROMIMG;
				}
				break;
			case ITEMID::WEAPON_WAND1:
				break;
			}
		}
	}
		



	return STATE::ROMIMG;
}

CPlayer_Attack* CPlayer_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CPlayer_Attack* pAttack = new CPlayer_Attack(pGraphicDev);

	if (FAILED(pAttack->Ready_State(pOwner)))
	{
		Safe_Release<CPlayer_Attack*>(pAttack);

		MSG_BOX("Create Attack State Failed");
		return nullptr;
	}

	return pAttack;
}

void CPlayer_Attack::Free()
{
	__super::Free();
}
