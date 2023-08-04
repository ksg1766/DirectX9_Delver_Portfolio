#include "HorizontalMove.h"
#include "Export_Function.h"
#include "Player.h"

CHorizontalMove::CHorizontalMove()
{
}

CHorizontalMove::CHorizontalMove(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CHorizontalMove::~CHorizontalMove()
{
}

HRESULT CHorizontalMove::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_bAttackCool = false;

	return S_OK;
}

STATE CHorizontalMove::Update_State(const _float& fTimeDelta)
{
	m_pOwner->Get_Animator()->Get_Animation()->Set_Loop(true);

	_vec3 vLook = m_pOwner->Get_Host()->m_pTransform->m_vInfo[INFO_LOOK];

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

		if(fDistance > 1.f)
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
			m_pOwner->Get_Host()->m_pTransform->Translate(_vec3(1.f, 0.f, 0.f) * 2 * fTimeDelta);
		else if (m_fChase < 20.f)
			m_pOwner->Get_Host()->m_pTransform->Translate(_vec3(-1.f, 0.f, 0.f) * 2 * fTimeDelta);
		else
			m_fChase = 0.f;
	}


	return STATE::ROMIMG;
}

void CHorizontalMove::LateUpdate_State()
{
}

void CHorizontalMove::Render_State()
{
}

CHorizontalMove* CHorizontalMove::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CHorizontalMove* pState = new CHorizontalMove(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CHorizontalMove*>(pState);

		MSG_BOX("Create Test Move Failed");
		return nullptr;
	}

	return pState;
}

void CHorizontalMove::Free()
{
	__super::Free();
}
