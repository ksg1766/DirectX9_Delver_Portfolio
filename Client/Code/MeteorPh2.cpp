#include "MeteorPh2.h"
#include "Export_Function.h"
#include "Player.h"
#include "SkeletonKing.h"
#include "SkeletonKing_CloneMeteor.h"
#include "Boss_LostSoul.h"
#include "Boss_MeteorCube.h"

CBoss_Meteor2Ph::CBoss_Meteor2Ph()
{
}

CBoss_Meteor2Ph::CBoss_Meteor2Ph(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_Meteor2Ph::~CBoss_Meteor2Ph()
{
}

HRESULT CBoss_Meteor2Ph::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
    m_iIndex = 0;
	m_bSkillStart = false;
    m_bSkill = false;
    m_bMove = false;
    m_bPosReset = false;
	m_fChannel_Count = 0.f;
    m_vPillarPos[0] = _vec3(-72.5f, 49.f, 124.f);//위
    m_vPillarPos[1] = _vec3(-104.f, 49.f, 94.5f);//좌
    m_vPillarPos[2] = _vec3(-38.f, 49.f, 94.5f);//우
	return S_OK;
}

STATE CBoss_Meteor2Ph::Update_State(const _float& fTimeDelta)
{
	m_fChannel_Count += fTimeDelta;
    m_fDelay += fTimeDelta;
    m_fPatternDelay += fTimeDelta;
	Engine::CGameObject* pGameObject = nullptr;
    if (!m_bSkill)
    {
        dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_CloneCount(0);
        if (!m_bPosReset)
        {
            if ((1.5f >= fabs(-72.5f - m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x)) && (1.5f >= fabs(94.5f - m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z)))
            {
                m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = _vec3(-72.5f, 38.f, 94.5f);
                m_fPatternDelay = 0.f;
                Make_Clone();
                m_bPosReset = true;
            }
            m_vDir = _vec3(-72.5f, 38.f, 94.5f) - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
            D3DXVec3Normalize(&m_vDir, &m_vDir);
            m_pOwner->Get_Transform()->Translate(m_vDir);
            m_fDelay = 0.f;
        }

    }
    else if ((2.f < m_fPatternDelay) && (m_bSkill)&&(!m_bMove))
    {
        m_fChannel_Count = 0.f;
        m_fDelay = 0.f;
        Move_DIr();
    }
    if (m_bMove)
    {
        m_bPosReset = false;
        if (!m_bSkillStart)
        {
            pGameObject = CBoss_MeteorCube::Create(m_pGraphicDev);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Set_Center(_vec3(-72.5f, 45.f, 94.5f));
            dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Set_State(STATE::BOSS_PH2SKILL5);
            dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Channeling_Begin();
            m_bSkillStart = true;
        }
        else if (m_bSkillStart && (12.1f < m_fChannel_Count))
        {
            m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y = 49.f;
            m_fDelay = 0.f;
            m_bSkillStart = false;
            m_fChannel_Count = 0.f;
            if (45 >= dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_BasicStat()->Get_Stat()->fHP)
            { 
                dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_Phase(BOSSPHASE::PHASE3);
                return STATE::BOSS_TELEPORT; 
            }
            else
            {
                m_fDelay = 0.f;
                m_fPatternDelay = 0.f;
                m_bSkillStart = false;
                m_bSkill = false;
                m_bMove = false;
                m_bPosReset = false;
                m_fChannel_Count = 0.f;
                return STATE::BOSS_IDLE;
            }
        }
        else if (m_bSkillStart)
        {
            m_pOwner->Get_Transform()->Translate(_vec3(0.f, 0.5f * fTimeDelta, 0.f));
        }
        if (1.8f < m_fDelay)
        {
            m_fDelay = 0.f;
            Make_LostSoul();
        }
    }
}

void CBoss_Meteor2Ph::LateUpdate_State()
{
}

void CBoss_Meteor2Ph::Render_State()
{
}

void CBoss_Meteor2Ph::Make_Clone()
{
    std::random_device rd;
    mt19937 engine(rd());
    uniform_int_distribution<__int64> distribution(0, 2); // 생성 범위
    auto generator = bind(distribution, engine);
    m_iIndex = generator();
    Engine::CGameObject* pGameObject = nullptr;
    Engine::CGameObject* pGameObject2 = nullptr;
    switch (m_iIndex)
    {
    case 0:
        m_vTargetPos = m_vPillarPos[m_iIndex];
        pGameObject = CSkeletonKing_CloneMeteor::Create(m_pGraphicDev);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->Set_bMeteor(true);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->Set_Dir(m_vPillarPos[1]);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

        pGameObject2 = CSkeletonKing_CloneMeteor::Create(m_pGraphicDev);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->Set_bMeteor(true);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->Set_Dir(m_vPillarPos[2]);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        Engine::EventManager()->CreateObject(pGameObject2, LAYERTAG::GAMELOGIC);
        m_bSkill = true;
        break;
    case 1:
        m_vTargetPos = m_vPillarPos[m_iIndex];
        pGameObject = CSkeletonKing_CloneMeteor::Create(m_pGraphicDev);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->Set_bMeteor(true);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->Set_Dir(m_vPillarPos[2]);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

        pGameObject2 = CSkeletonKing_CloneMeteor::Create(m_pGraphicDev);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->Set_bMeteor(true);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->Set_Dir(m_vPillarPos[0]);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        Engine::EventManager()->CreateObject(pGameObject2, LAYERTAG::GAMELOGIC);
        m_bSkill = true;
        break;
    case 2:
        m_vTargetPos = m_vPillarPos[m_iIndex];
        pGameObject = CSkeletonKing_CloneMeteor::Create(m_pGraphicDev);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->Set_bMeteor(true);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->Set_Dir(m_vPillarPos[1]);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

        pGameObject2 = CSkeletonKing_CloneMeteor::Create(m_pGraphicDev);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->Set_bMeteor(true);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->Set_Dir(m_vPillarPos[0]);
        dynamic_cast<CSkeletonKing_CloneMeteor*>(pGameObject2)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        Engine::EventManager()->CreateObject(pGameObject2, LAYERTAG::GAMELOGIC);
        m_bSkill = true;
        break;
    }
}

void CBoss_Meteor2Ph::Make_LostSoul()
{
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CBossLostSoul::Create(m_pGraphicDev);
	dynamic_cast<CBossLostSoul*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
	dynamic_cast<CBossLostSoul*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
}


void CBoss_Meteor2Ph::Move_DIr()
{
    m_vDir = m_vTargetPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    m_pOwner->Get_Transform()->Translate(m_vDir);
    if (1.f < m_fPatternDelay)
    {
        m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_vTargetPos;
        m_fDelay = 0.f;
        m_fPatternDelay = 0.f;
        m_bMove = true;
    }
}

CBoss_Meteor2Ph* CBoss_Meteor2Ph::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBoss_Meteor2Ph* pState = new CBoss_Meteor2Ph(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBoss_Meteor2Ph*>(pState);

		MSG_BOX("Meteor2 State Failed");
	}
	return pState;
}

void CBoss_Meteor2Ph::Free()
{
	__super::Free();
}