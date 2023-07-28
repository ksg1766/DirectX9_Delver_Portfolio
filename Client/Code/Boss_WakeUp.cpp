#include "Boss_WakeUp.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
CBoss_WakeUp::CBoss_WakeUp()
{
}

CBoss_WakeUp::CBoss_WakeUp(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_WakeUp::~CBoss_WakeUp()
{
}

HRESULT CBoss_WakeUp::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_bWakeUp = false;
    m_fDelay = 0.f;
    return S_OK;
}

STATE CBoss_WakeUp::Update_State(const _float& fTimeDelta)
{
    m_fDelay += fTimeDelta;
    if ((1.4f > m_fDelay) && (!m_bWakeUp))
        m_bWakeUp = true;
    if ((1.4f <= m_fDelay)&&(m_bWakeUp))
    {
        m_fDelay = 0.f;
        m_bWakeUp = false;
        return STATE::BOSS_IDLE;
    }
}

void CBoss_WakeUp::LateUpdate_State()
{
}

void CBoss_WakeUp::Render_State()
{
}

CBoss_WakeUp* CBoss_WakeUp::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_WakeUp* pState = new CBoss_WakeUp(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_WakeUp*>(pState);

        MSG_BOX("Boss WakeUp State Failed");
    }
    return pState;
}

void CBoss_WakeUp::Free()
{
    __super::Free();
}
