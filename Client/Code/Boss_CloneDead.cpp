#include "Boss_CloneDead.h"
#include "Export_Function.h"
#include "SkeletonKing_Clone.h"
#include "BossExplosion.h"
CBoss_CloneDead::CBoss_CloneDead()
{
}

CBoss_CloneDead::CBoss_CloneDead(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_CloneDead::~CBoss_CloneDead()
{
}

HRESULT CBoss_CloneDead::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_fDeadDelay = 0.f;
	return S_OK;
}

STATE CBoss_CloneDead::Update_State(const _float& fTimeDelta)
{
    m_fDeadDelay += fTimeDelta;
	return STATE::DEAD;
}

void CBoss_CloneDead::LateUpdate_State()
{
}

void CBoss_CloneDead::Render_State()
{
}

CBoss_CloneDead* CBoss_CloneDead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_CloneDead* pState = new CBoss_CloneDead(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_CloneDead*>(pState);

        MSG_BOX("Boss Attack State Failed");
    }
    return pState;
}

void CBoss_CloneDead::Free()
{
    __super::Free();
}
