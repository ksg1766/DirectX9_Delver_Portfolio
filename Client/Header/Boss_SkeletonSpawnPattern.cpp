#include "Boss_SkeletonSpawnPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Player.h"
#include "Skeleton.h"
#include "SkullGhost.h"
#include "Boss_CautionEff.h"
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
    m_bCaution = false;
    m_fSkillCool = 0.f;
    m_vSpawnPos1[0] = _vec3(-6.f, 0.f, 6.f);//보스 우측상단
    m_vSpawnPos1[1] = _vec3(-6.f, 0.f, -6.f);//보스 좌측상단
    m_vSpawnPos1[2] = _vec3(6.f, 0.f, 6.f);//보스 우측하단
    m_vSpawnPos1[3] = _vec3(6.f, 0.f, -6.f);//보스 좌측하단
    m_vSpawnPos1[4] = _vec3(-6.f, 0.f, 0.f);
    m_vSpawnPos1[5] = _vec3(0.f, 0.f, -6.f);
    m_vSpawnPos1[6] = _vec3(6.f, 0.f, 0.f);
    m_vSpawnPos1[7] = _vec3(0.f, 0.f, 6.f);
    return S_OK;
}

STATE CBoss_SkeletonSpawnPattern::Update_State(const _float& fTimeDelta)
{
    Engine::CGameObject* pGameObject = nullptr;
    m_fSkillCool += fTimeDelta;
    if ((1.f < m_fSkillCool)&&(!m_bCaution))
    {
       for (int j = 0; j < 8; ++j)
       {
           for (int i = 0; i < 10; ++i)
           {
               pGameObject = CBoss_CautionEff::Create(m_pGraphicDev);
               //static_cast<CBoss_CautionEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_vSpawnPos1[j].x, 35.f + (i * 2), m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + m_vSpawnPos1[j].z);
               static_cast<CBoss_CautionEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(Engine::SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS].x + m_vSpawnPos1[j].x, 35.f + (i * 2),
                   Engine::SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS].z + m_vSpawnPos1[j].z);
               Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
           }
       }
        m_bCaution = true;
    }
    if ((1.5f < m_fSkillCool) && (!m_bSkill))
    {
        Spawn_Skeleton();
        m_bSkill = true;
    }
    if (3.f < m_fSkillCool)
    {
        m_bSkill = false;
        m_bCaution = false;
        m_fSkillCool = 0.f;
        return STATE::BOSS_IDLE;
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

    for (int i = 0; i < 8; ++i)
    {
        pGameObject = CSkeleton::Create(m_pGraphicDev);
       // static_cast<CSkeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + m_vSpawnPos1[i];
        static_cast<CSkeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = Engine::SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS] + m_vSpawnPos1[i];
        static_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->iExpMax = 0.f;
        static_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->iExp = 0.f;
        static_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->fHP = 3.f;
        static_cast<CSkeleton*>(pGameObject)->Set_CenterPos(_vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_vSpawnPos1[i].x/2 ,35.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + m_vSpawnPos1[i].z/2));
        static_cast<CSkeleton*>(pGameObject)->Set_MoveRange(10.f);
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
    __super::Free();
}
