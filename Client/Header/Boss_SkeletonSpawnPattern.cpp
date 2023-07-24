#include "Boss_SkeletonSpawnPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Player.h"
#include "Skeleton.h"
#include "SkullGhost.h"
#include "Alien.h"
CBoss_SkeletonSpawnPattern::CBoss_SkeletonSpawnPattern()
{
}

CBoss_SkeletonSpawnPattern::CBoss_SkeletonSpawnPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_SkeletonSpawnPattern::~CBoss_SkeletonSpawnPattern()
{
}

HRESULT CBoss_SkeletonSpawnPattern::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_bSkill = false;
    m_fSkillCool = 0.f;
    m_vSpawnPos1[0] = _vec3(-10.f, 0.f, 10.f);//보스 우측상단
    m_vSpawnPos1[1] = _vec3(-10.f, 0.f, -10.f);//보스 좌측상단
    m_vSpawnPos2[0] = _vec3(10.f, 0.f, 10.f);//보스 우측하단
    m_vSpawnPos2[1] = _vec3(10.f, 0.f, -10.f);//보스 좌측하단
    return S_OK;
}

STATE CBoss_SkeletonSpawnPattern::Update_State(const _float& fTimeDelta)
{
    Engine::CGameObject* pGameObject = nullptr;
    m_fSkillCool += fTimeDelta;
    if ((1.5f < m_fSkillCool) && (!m_bSkill))
    {
        Spawn_Skeleton();
        m_bSkill = true;
    }
    if (3.f < m_fSkillCool)
    {
        m_bSkill = false;
        m_fSkillCool = 0.f;
        return STATE::BOSS_PH1SKILL4;
    }
}

void CBoss_SkeletonSpawnPattern::LateUpdate_State()
{
}

void CBoss_SkeletonSpawnPattern::Render_State()
{
}

void CBoss_SkeletonSpawnPattern::Spawn_Skeleton()
{
    Engine::CGameObject* pGameObject = nullptr;
    for (int i = 0; i < 2; ++i)
    {
        pGameObject = CSkeleton::Create(m_pGraphicDev);
        dynamic_cast<CSkeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vSpawnPos2[i];
        dynamic_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->fHP = 2.f;
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        pGameObject = CSkeleton::Create(m_pGraphicDev);
        dynamic_cast<CSkeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vSpawnPos1[i];
        dynamic_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->fHP = 2.f;
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
    }
 
}

CBoss_SkeletonSpawnPattern* CBoss_SkeletonSpawnPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_SkeletonSpawnPattern* pState = new CBoss_SkeletonSpawnPattern(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_SkeletonSpawnPattern*>(pState);

        MSG_BOX("Boss SpawnPattern State Failed");
    }
    return pState;
}

void CBoss_SkeletonSpawnPattern::Free()
{
}
