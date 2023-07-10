#include "Boss_Sturn.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
CBoss_Sturn::CBoss_Sturn()
{
}

CBoss_Sturn::CBoss_Sturn(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_Sturn::~CBoss_Sturn()
{
}

HRESULT CBoss_Sturn::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fCount = 0.f;
    return S_OK;
}

STATE CBoss_Sturn::Update_State(const _float& fTimeDelta)
{
    m_fCount += fTimeDelta;
    if (3.f < m_fCount)
    {
        m_fCount = 0.f;
        return STATE::BOSS_IDLE;
    }
}

void CBoss_Sturn::LateUpdate_State()
{
}

void CBoss_Sturn::Render_State()
{
}

CBoss_Sturn* CBoss_Sturn::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Sturn* pState = new CBoss_Sturn(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Sturn*>(pState);

        MSG_BOX("Boss Sturn State Failed");
    }
    return pState;
}

void CBoss_Sturn::Free()
{
    __super::Free();
}
