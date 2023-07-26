#include "stdafx.h"
#include "..\Header\Player_Attack.h"
#include "Export_Function.h"
#include "SoundManager.h"
#include "Player.h"
#include "Item.h"
#include "Itemgroup.h"
#include "FireBall.h"
#include "EffectProjectileTrace.h"

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
	m_fSpeed = 0.f;

	return S_OK;
}

STATE CPlayer_Attack::Update_State(const _float& fTimeDelta)
{
	STATE eState = STATE::IDLE;

	if(!dynamic_cast<CPlayer*>(m_pOwner->Get_Host())->Get_UseUI())
		eState = Key_Input(fTimeDelta);

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

	CPlayer& pPlayer = *dynamic_cast<CPlayer*>(SceneManager()->Get_Scene()->Get_MainPlayer());

	if (!pPlayer.IsTalk())
	{
		if (Engine::InputDev()->Key_Pressing(DIK_W))
		{
			_vec3 vFront = vLook;
			vFront.y = 0.f;
			m_pOwner->Get_Transform()->Translate(6.f * fTimeDelta * vFront);
		}

		if (Engine::InputDev()->Key_Pressing(DIK_S))
		{
			_vec3 vFront = vLook;
			vFront.y = 0.f;
			m_pOwner->Get_Transform()->Translate(6. * fTimeDelta * -vFront);
		}

		if (Engine::InputDev()->Key_Pressing(DIK_A))
		{
			_vec3 vSide = vRight;
			vSide.y = 0.f;
			m_pOwner->Get_Transform()->Translate(6.f * fTimeDelta * -vSide);
		}

		if (Engine::InputDev()->Key_Pressing(DIK_D))
		{
			_vec3 vSide = vRight;
			vSide.y = 0.f;
			m_pOwner->Get_Transform()->Translate(6.f * fTimeDelta * vSide);
		}

		if (!pPlayer.Get_Attack() && pPlayer.Get_CurrentEquipRight() != nullptr)
		{
			ITEMTYPEID _eID = dynamic_cast<CItem*>(pPlayer.Get_CurrentEquipRight())->Get_ItemTag();

			switch (_eID.eItemID)
			{
			case ITEMID::WEAPON_SWORD:
				if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
				{
					pPlayer.Set_Attack(true);
					pPlayer.Set_AttackTick(false);
					pPlayer.Set_State(STATE::ATTACK);


					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WEAPON);
					CSoundManager::GetInstance()->PlaySound(L"whoosh1.mp3", CHANNELID::SOUND_WEAPON, 1.f);

					return STATE::ATTACK;
				}
				break;
			case ITEMID::WEAPON_BOW:
				if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
				{
					pPlayer.Set_Attack(true);
					pPlayer.Set_State(STATE::ATTACK);
					m_fSpeed += 1.3f;

					if (m_fSpeed >= 20)
						m_fSpeed = 20.f;

					CSoundManager::GetInstance()->PlaySound(L"wep_swing_bow_load_01.mp3", CHANNELID::SOUND_WEAPON, 1.f);

					_eState = STATE::ATTACK;
				}
				if (Engine::InputDev()->Mouse_Up(DIM_LB))
				{
					// TODO : 마우스를 놓으면 화살 날라감.(Preesing에서 누르만큼 +Speed)
					// CreateArrow. -> 내가 보는 방향으로 날리기.
					if (m_fSpeed >= 20.f)
					{
						CGameObject* pGameObject = nullptr;
						pGameObject = CArrow::Create(m_pGraphicDev,
							dynamic_cast<CBow*>(pPlayer.Get_CurrentEquipRight())->m_pTransform,
							m_pOwner->Get_Transform(), m_fSpeed);

						Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);


					}


					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WEAPON);
					CSoundManager::GetInstance()->PlaySound(L"bow.wav", CHANNELID::SOUND_WEAPON, 1.f);
					m_fSpeed = 0.f;

					pPlayer.Set_State(STATE::ROMIMG);
					_eState = STATE::ROMIMG;
;
				}
				break;
			case ITEMID::WEAPON_WAND3:
				if (Engine::InputDev()->Mouse_Down(DIM_LB))
				{
					pPlayer.Set_Attack(true);
					pPlayer.Set_AttackTick(false);
					pPlayer.Set_State(STATE::ATTACK);

					CGameObject* pGameObject = nullptr;
					pGameObject = CFireBall::Create(m_pGraphicDev,
						dynamic_cast<CFireWands*>(pPlayer.Get_CurrentEquipRight())->m_pTransform,
						m_pOwner->Get_Transform(), 10.f);

					Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

					CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WEAPON);
					CSoundManager::GetInstance()->PlaySound(L"explode.mp3", CHANNELID::SOUND_WEAPON, 1.f);

					_eState = STATE::ATTACK;
				}
				break;
			case ITEMID::WEAPON_EPICBOW:
				if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
				{
					pPlayer.Set_Attack(true);
					pPlayer.Set_State(STATE::ATTACK);
					m_fSpeed += 1.3f;

					if (m_fSpeed >= 20)
						m_fSpeed = 20.f;

					_eState = STATE::ATTACK;
					CSoundManager::GetInstance()->PlaySound(L"wep_swing_bow_load_01.mp3", CHANNELID::SOUND_WEAPON, 1.f);
				}
				if (Engine::InputDev()->Mouse_Up(DIM_LB))
				{
					// TODO : 마우스를 놓으면 화살 날라감.(Preesing에서 누르만큼 +Speed)
					// CreateArrow. -> 내가 보는 방향으로 날리기.
					if (m_fSpeed >= 20.f)
					{
						CGameObject* pGameObject = nullptr;
						pGameObject = CArrow::Create(m_pGraphicDev,
							dynamic_cast<CEpicBow*>(pPlayer.Get_CurrentEquipRight())->m_pTransform,
							m_pOwner->Get_Transform(), m_fSpeed);

						Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);


						CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_WEAPON);
						CSoundManager::GetInstance()->PlaySound(L"explode.mp3", CHANNELID::SOUND_EFFECT, 1.f);
					}

					m_fSpeed = 0.f;

					pPlayer.Set_State(STATE::ROMIMG);
					_eState = STATE::ROMIMG;
				}
				break;
			}
		}

		return _eState;
	}

	return _eState;
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
