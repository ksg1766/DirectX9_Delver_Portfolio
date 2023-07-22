#include "MeteorPh2.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"

CBoss_Meteor2Ph::CBoss_Meteor2Ph()
{
}

CBoss_Meteor2Ph::CBoss_Meteor2Ph(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_Meteor2Ph::~CBoss_Meteor2Ph()
{
}

HRESULT CBoss_Meteor2Ph::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_bCool = false;
	return S_OK;
}

STATE CBoss_Meteor2Ph::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;




	return STATE::BOSS_IDLE;
}

void CBoss_Meteor2Ph::LateUpdate_State()
{
}

void CBoss_Meteor2Ph::Render_State()
{
}

CBoss_Meteor2Ph* CBoss_Meteor2Ph::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBoss_Meteor2Ph* pState = new CBoss_Meteor2Ph(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBoss_Meteor2Ph*>(pState);

		MSG_BOX("Fire State Failed");
	}
	return pState;
}

void CBoss_Meteor2Ph::Free()
{
	__super::Free();
}
