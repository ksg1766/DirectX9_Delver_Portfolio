#include "Boss_CloneMeteorDead.h"
#include "Export_Function.h"
#include "SkeletonKing_CloneMeteor.h"
#include "BossExplosion.h"
CBoss_CloneMeteorDead::CBoss_CloneMeteorDead()
{
}

CBoss_CloneMeteorDead::CBoss_CloneMeteorDead(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_CloneMeteorDead::~CBoss_CloneMeteorDead()
{
}

HRESULT CBoss_CloneMeteorDead::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_fDeadDelay = 0.f;
	return S_OK;
}

STATE CBoss_CloneMeteorDead::Update_State(const _float& fTimeDelta)
{
    m_fDeadDelay += fTimeDelta;
	return STATE::DEAD;
}

void CBoss_CloneMeteorDead::LateUpdate_State()
{
}

void CBoss_CloneMeteorDead::Render_State()
{
}

CBoss_CloneMeteorDead* CBoss_CloneMeteorDead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_CloneMeteorDead* pState = new CBoss_CloneMeteorDead(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_CloneMeteorDead*>(pState);

        MSG_BOX("Boss Attack State Failed");
    }
    return pState;
}

void CBoss_CloneMeteorDead::Free()
{
    __super::Free();
}
