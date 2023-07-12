#include "Boss_Idle.h"
#include "Export_Function.h"
CBoss_Idle::CBoss_Idle()
{
}

CBoss_Idle::CBoss_Idle(LPDIRECT3DDEVICE9 gGraphicDev)
    : CState(gGraphicDev)
{
}

CBoss_Idle::~CBoss_Idle()
{
}

HRESULT CBoss_Idle::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fSkillCool = 0.f;
    return S_OK;
}

STATE CBoss_Idle::Update_State(const _float& fTimeDelta)
{
    m_fSkillCool += fTimeDelta;
    if (3.f < m_fSkillCool)
    {
        m_fSkillCool = 0.f;
        return STATE::BOSS_ATTACK;
    }
    return STATE::BOSS_IDLE;
}

void CBoss_Idle::LateUpdate_State()
{
}

void CBoss_Idle::Render_State()
{
    
}

CBoss_Idle* CBoss_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Idle* pState = new CBoss_Idle(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Idle*>(pState);

        MSG_BOX("Boss Idle State Failed");
    }
    return pState;
}

void CBoss_Idle::Free()
{
    __super::Free();
}
