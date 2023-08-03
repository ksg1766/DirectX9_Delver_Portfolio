#include "stdafx.h"
#include "SoundManager.h"
#include "Boss_Dying.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "GameManager.h"
CBoss_Dying::CBoss_Dying()
{
}

CBoss_Dying::CBoss_Dying(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_Dying::~CBoss_Dying()
{
}

HRESULT CBoss_Dying::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fDelay = 0.f;
    m_bSound = false;
    return S_OK;
}

STATE CBoss_Dying::Update_State(const _float& fTimeDelta)
{
    m_fDelay += fTimeDelta;
    if (2.f >= m_fDelay)
        m_pOwner->Get_Animator()->Get_Animation()->Set_Frame(0);
    
    if ((2.f < m_fDelay) && (!m_bSound))
    {
        CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BOSS);
        CSoundManager::GetInstance()->PlaySound(L"Boss_Death2.wav", CHANNELID::SOUND_BOSS, 1.f);
    }

    _float fFrame = m_pOwner->Get_Animator()->Get_Animation()->Get_Frame();
    if (11.f <= fFrame)
        return STATE::BOSS_DEAD;
}

void CBoss_Dying::LateUpdate_State()
{
}

void CBoss_Dying::Render_State()
{
}

CBoss_Dying* CBoss_Dying::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Dying* pState = new CBoss_Dying(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Dying*>(pState);

        MSG_BOX("Boss Dying State Failed");
    }
    return pState;
}

void CBoss_Dying::Free()
{
    __super::Free();
}
