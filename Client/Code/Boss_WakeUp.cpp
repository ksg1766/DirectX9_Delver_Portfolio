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
    return S_OK;
}

STATE CBoss_WakeUp::Update_State(const _float& fTimeDelta)
{
    _float fFrame = m_pOwner->Get_Animator()->Get_Animation()->Get_Frame();
    if (15.f < fFrame)
        return STATE::BOSS_IDLE;
    else 
        return STATE::BOSS_WAKEUP;
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
