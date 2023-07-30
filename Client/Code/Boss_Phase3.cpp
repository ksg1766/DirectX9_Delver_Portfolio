#include "stdafx.h"
#include "Boss_Phase3.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "BossProjectile.h"
#include "BossExplosion.h"
#include "SkeletonKing_Clone.h"
#include "SoundManager.h"
CBoss_Phase3::CBoss_Phase3()
{
}

CBoss_Phase3::CBoss_Phase3(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_Phase3::~CBoss_Phase3()
{
}

HRESULT CBoss_Phase3::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fDelay = 0.f;
    m_iSkillCount = 0;
    m_fPatternDelay = 0.f;
    return S_OK;
}

STATE CBoss_Phase3::Update_State(const _float& fTimeDelta)
{
    if (BOSSPHASE::PHASE3 != dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase())
        return STATE::BOSS_IDLE;
    m_fDelay += fTimeDelta;
    m_fPatternDelay += fTimeDelta;
    if (3.f < m_fDelay)
    {
        if (!dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_3Phase())
            dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_3Phase(true);
        m_fDelay = 0.f;
        return BossSkill(fTimeDelta);
    }
}

void CBoss_Phase3::LateUpdate_State()
{

}

void CBoss_Phase3::Render_State()
{
}

STATE CBoss_Phase3::BossSkill(const _float& fTimeDelta)
{
    switch(m_iSkillCount)
    {
    case 0 :
        ++m_iSkillCount;
        CSoundManager::GetInstance()->PlaySound(L"Boss_Roar1.wav",SOUND_BOSS,1.f);
        return STATE::BOSS_PH3SKILL1;
        break;
    case 1:
        ++m_iSkillCount;
        CSoundManager::GetInstance()->PlaySound(L"Boss_Roar2.wav", SOUND_BOSS, 1.f);
        return STATE::BOSS_PH3SKILL2;
        break;
    case 2:
        ++m_iSkillCount;
        CSoundManager::GetInstance()->PlaySound(L"Boss_Roar3.wav", SOUND_BOSS, 1.f);
        return STATE::BOSS_PH3SKILL3;
        break;
    case 3:
        ++m_iSkillCount;
        CSoundManager::GetInstance()->PlaySound(L"Boss_Power1.wav", SOUND_BOSS, 1.f);
        return STATE::BOSS_PH3SKILL4;
        break;
    case 4:
        if ((30.f > dynamic_cast<CSkeletonKing*>(m_pOwner->Get_Host())->Get_BasicStat()->Get_Stat()->fHP))
        {
            dynamic_cast<CSkeletonKing*>(m_pOwner->Get_Host())->ReSet_Sturn();
            CSoundManager::GetInstance()->PlaySound(L"Boss_Power2.wav", SOUND_BOSS, 1.f);
            m_fPatternDelay = 0.f;
            m_iSkillCount = 0;
            return STATE::BOSS_PH3SKILL5;
        }
        else
        {
            m_iSkillCount = 0;
            break;
            return STATE::BOSS_IDLE;
        }
    }
}

CBoss_Phase3* CBoss_Phase3::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Phase3* pState = new CBoss_Phase3(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Phase3*>(pState);

        MSG_BOX("Boss Attack State Failed");
    }
    return pState;
}

void CBoss_Phase3::Free()
{
    __super::Free();
}
