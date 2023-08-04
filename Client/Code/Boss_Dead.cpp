#include "stdafx.h"
#include "SoundManager.h"
#include "Boss_Dead.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "DimensionGate.h"
#include "UIbosshp.h"
#include "UIManager.h"
CBoss_Dead::CBoss_Dead()
{
}

CBoss_Dead::CBoss_Dead(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_Dead::~CBoss_Dead()
{
}

HRESULT CBoss_Dead::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_bSpawnGate = false;
    m_fSound = 1.f;
    return S_OK;
}

STATE CBoss_Dead::Update_State(const _float& fTimeDelta)
{
    if (0.f < m_fSound)
    {
        m_fSound -= (fTimeDelta / 10.f);
        CSoundManager::GetInstance()->SetChannelVolume(CHANNELID::SOUND_BGM, m_fSound);
    }
    if (0.f >= m_fSound)
    {
        CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_BGM);
        Engine::UIManager()->Hide_PopupUI(Engine::UIPOPUPLAYER::POPUP_BOSSHP);
    }
    
    return STATE::BOSS_DEAD;

}

void CBoss_Dead::LateUpdate_State()
{
}

void CBoss_Dead::Render_State()
{
}

CBoss_Dead* CBoss_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Dead* pState = new CBoss_Dead(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Dead*>(pState);

        MSG_BOX("Boss Dead State Failed");
    }
    return pState;
}

void CBoss_Dead::Free()
{
    __super::Free();
}
