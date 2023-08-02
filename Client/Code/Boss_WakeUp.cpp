#include "stdafx.h"
#include "Boss_WakeUp.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SoundManager.h"
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
    if (BOSSPHASE::LASTPHASE != dynamic_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_Phase())
    {
        if ((0.1f > m_fDelay) && (!m_bWakeUp))
        {
            CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BOSS);
            CSoundManager::GetInstance()->PlaySound(L"Boss_Laugh1.wav", CHANNELID::SOUND_BOSS, 1.f);
            dynamic_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_StateMachine()->Get_Animator()->Get_Animation()->Set_Frame(0.f);
        }
        m_fDelay += fTimeDelta;
        if ((1.42f > m_fDelay) && (!m_bWakeUp))
            m_bWakeUp = true;
        if ((1.42f <= m_fDelay) && (m_bWakeUp))
        {
            m_fDelay = 0.f;
            m_bWakeUp = false;
            return STATE::BOSS_IDLE;
        }
    }

    if (BOSSPHASE::LASTPHASE == dynamic_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_Phase())
    {
        if ((0.1f > m_fDelay) && (!m_bWakeUp))
        {
            CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BOSS);
            CSoundManager::GetInstance()->PlaySound(L"Boss_Death1.wav", CHANNELID::SOUND_BOSS, 1.f);
            dynamic_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_StateMachine()->Get_Animator()->Get_Animation()->Set_Frame(0.f);
        }
        m_fDelay += fTimeDelta;
        if ((1.42f > m_fDelay) && (!m_bWakeUp))
            m_bWakeUp = true;
        if ((1.42f <= m_fDelay) && (m_bWakeUp))
        {
            m_fDelay = 0.f;
            m_bWakeUp = false;
            return STATE::BOSS_CRAWL;
        }
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
