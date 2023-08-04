#include "Longitudinal.h"
#include "Export_Function.h"
#include "Player.h"

CLongitudinal::CLongitudinal()
{
}

CLongitudinal::CLongitudinal(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CLongitudinal::~CLongitudinal()
{
}

HRESULT CLongitudinal::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bAttackCool = false;
	m_fAttackCool = 0.f;

	return S_OK;
}

STATE CLongitudinal::Update_State(const _float& fTimeDelta)
{
	_vec3 vLook = m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_LOOK];

	m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(true);

	CPlayer& pPlayer =
		*static_cast<CPlayer*>
		(SceneManager()->GetInstance()->Get_ObjectList
		(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front());

	if (m_pOwner->Get_PrevState() == STATE::ATTACK)
	{
		m_fAttackCool += fTimeDelta;

		if (m_fAttackCool > 2.f)
			m_bAttackCool = false;

	}

	_float fDistance = D3DXVec3Length(&(pPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pOwner->Get_Transform()->m_vInfo[INFO_POS]));

	if (fDistance < 20.f)
	{
		_vec3 vDir = pPlayer.m_pTransform->m_vInfo[INFO_POS] - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&vDir, &vDir);

		if (fDistance > 2.f)
			m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + vDir * 5 * fTimeDelta;

		if (fDistance < 5.f && !Get_AttackCool())
		{
			m_fAttackCool = 0.f;
			m_bAttackCool = true;
			return STATE::ATTACK;
		}

		return STATE::ROMIMG;
	}
	else
	{
		m_fChase += fTimeDelta;

		if (m_fChase < 10.f)
			m_pOwner->Get_Host()->m_pTransform->Translate(_vec3(0.f, 0.f, 1.f) * 2 * fTimeDelta);
		else if (m_fChase < 20.f)
			m_pOwner->Get_Host()->m_pTransform->Translate(_vec3(0.f, 0.f, -1.f) * 2 * fTimeDelta);
		else
			m_fChase = 0.f;
	}


	return STATE::ROMIMG;
}

void CLongitudinal::LateUpdate_State()
{
}

void CLongitudinal::Render_State()
{
}

CLongitudinal* CLongitudinal::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CLongitudinal* pState = new CLongitudinal(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CLongitudinal*>(pState);

		MSG_BOX("Create Test Move Failed");
		return nullptr;
	}

	return pState;
}

void CLongitudinal::Free()
{
	__super::Free();
}
