#include "Boss_Phase1.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "BossProjectile.h"
#include "BossExplosion.h"
#include "SkeletonKing_Clone.h"
CBoss_Phase1::CBoss_Phase1()
{
}

CBoss_Phase1::CBoss_Phase1(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_Phase1::~CBoss_Phase1()
{
}

HRESULT CBoss_Phase1::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fDelay = 0.f;
    m_iSkillCount = 0;
    return S_OK;
}

STATE CBoss_Phase1::Update_State(const _float& fTimeDelta)
{
    if (BOSSPHASE::PHASE1 != dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_Phase())
        return STATE::BOSS_IDLE;
    m_fDelay += fTimeDelta;
    if (3.f < m_fDelay)
    {
        m_fDelay = 0.f;
        return BossSkill(fTimeDelta);
    }
}

void CBoss_Phase1::LateUpdate_State()
{

}

void CBoss_Phase1::Render_State()
{
}

STATE CBoss_Phase1::BossSkill(const _float& fTimeDelta)
{
    switch(m_iSkillCount)
    {
    case 0 :
        ++m_iSkillCount;
        return STATE::BOSS_PH1SKILL1;
        break;
    case 1:
        ++m_iSkillCount;
        return STATE::BOSS_PH1SKILL2;
        break;
    case 2:
        ++m_iSkillCount;
        return STATE::BOSS_PH1SKILL3;
        break;
    case 3:
        ++m_iSkillCount;
        return STATE::BOSS_PH1SKILL4;
        break;
    case 4:
        if ((70.f >= dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_BasicStat()->Get_Stat()->fHP))
        {
            m_iSkillCount = 0;
            return STATE::BOSS_PH1SKILL5;
            break;
        }
        else
        {
            m_iSkillCount = 0;
            return STATE::BOSS_IDLE;
        }
        break;
    }


}

CBoss_Phase1* CBoss_Phase1::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Phase1* pState = new CBoss_Phase1(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Phase1*>(pState);

        MSG_BOX("Boss Attack State Failed");
    }
    return pState;
}

void CBoss_Phase1::Free()
{
    __super::Free();
}
