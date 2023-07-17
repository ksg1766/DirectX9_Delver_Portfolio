#include "..\Header\Charge.h"
#include "Export_Function.h"
#include "Player.h"

CCharge::CCharge()
{
}

CCharge::CCharge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CCharge::~CCharge()
{
}

HRESULT CCharge::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChase = 10.f;
	m_vPrevPos = _vec3(0.f, 0.f, 0.f);
	m_bIsAttack = false;
	m_fSpeed = 10.f;
	m_CoolTime = 0.f;
	m_bAttackTick = false;
	m_bAttackTick2 = false;
	m_bCheck = false;
	return S_OK;
}

STATE CCharge::Update_State(const _float& fTimeDelta)
{
	CPlayer& pPlayer =
		*dynamic_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());


	if (!m_bIsAttack)
	{
		m_fChase += fTimeDelta;

		if (m_fChase < 1.f)
		{
			m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(1.f);
			m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(false);
		}
		else
		{
			dynamic_cast<CMonster*>(m_pOwner->Get_Host())->Set_AttackTick(false);
			m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(true);
			m_vPrevPos = pPlayer.m_pTransform->m_vInfo[INFO_POS];
			m_bIsAttack = true;
			m_fChase = 0.f;
		}
	}

	if (m_bIsAttack)
	{
		_vec3 vDir = m_vPrevPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vDir, &vDir);

		m_pOwner->Get_Transform()->m_vInfo[INFO_POS] =
			m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + vDir * 10;
	}


	if (D3DXVec3Length(&(m_vPrevPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS])) < 0.5f
		&& m_pOwner->Get_Animator()->Get_Animation()->Get_Frame() > 3.8f)
	{
		m_bIsAttack = false;
		return STATE::IDLE;
	}
}

void CCharge::LateUpdate_State()
{
}

void CCharge::Render_State()
{
	//cout << "Spider Jump" << endl;
}


CCharge* CCharge::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CCharge* pState = new CCharge(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CCharge*>(pState);

		MSG_BOX("Create Jump Failed");
		return nullptr;
	}

	return pState;
}

void CCharge::Free()
{
	__super::Free();
}
