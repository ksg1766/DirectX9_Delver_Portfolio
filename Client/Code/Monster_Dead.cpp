#include "..\Header\Monster_Dead.h"
#include "Export_Function.h"
#include "Player.h"

CMonster_Dead::CMonster_Dead()
{
}

CMonster_Dead::CMonster_Dead(LPDIRECT3DDEVICE9 pGraphicDev)
	: CState(pGraphicDev)
{
}

CMonster_Dead::~CMonster_Dead()
{
}

HRESULT CMonster_Dead::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fFlyDown = 7.f;

	return S_OK;
}

STATE CMonster_Dead::Update_State(const _float& fTimeDelta)
{

	if (m_pOwner->Get_Host()->Get_MonsterTag() == MONSTERTAG::BAT)
	{
		m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y += m_fFlyDown * fTimeDelta;

		m_fFlyDown -= 0.5f * fTimeDelta * fTimeDelta * 3000.f;

		if (m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y < 1.f)
			m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y = 0.5f;
	}

	return STATE::DEAD;
}

void CMonster_Dead::LateUpdate_State()
{
}

void CMonster_Dead::Render_State()
{
}

CMonster_Dead* CMonster_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMonster_Dead* pState = new CMonster_Dead(pGraphicDev);

	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMonster_Dead*>(pState);

		MSG_BOX("Create Monster Dead Failed");
		return nullptr;
	}

	return pState;
}

void CMonster_Dead::Free()
{
	__super::Free();
}
