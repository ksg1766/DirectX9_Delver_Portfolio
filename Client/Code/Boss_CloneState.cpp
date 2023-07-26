#include "Boss_CloneState.h"
#include "Export_Function.h"
#include "SkeletonKing_Clone.h"
#include "BossProjectile.h"
#include "SkeletonKing.h"
#include "Player.h"
CBoss_CloneState::CBoss_CloneState()
{
}

CBoss_CloneState::CBoss_CloneState(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_CloneState::~CBoss_CloneState()
{
}

HRESULT CBoss_CloneState::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_fDelay = 0.f;
	return S_OK;
}

STATE CBoss_CloneState::Update_State(const _float& fTimeDelta)
{
    m_fDelay += fTimeDelta;
    if (3.f < m_fDelay)
    {
        Engine::CGameObject* pGameObject = nullptr;
        pGameObject = CBossProjectile::Create(m_pGraphicDev);
        dynamic_cast<CBossProjectile*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        m_fDelay = 0.f;
	    return STATE::IDLE;
    }
}

void CBoss_CloneState::LateUpdate_State()
{
}

void CBoss_CloneState::Render_State()
{
}

CBoss_CloneState* CBoss_CloneState::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_CloneState* pState = new CBoss_CloneState(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_CloneState*>(pState);

        MSG_BOX("Boss Attack State Failed");
    }
    return pState;
}

void CBoss_CloneState::Free()
{
    __super::Free();
}
