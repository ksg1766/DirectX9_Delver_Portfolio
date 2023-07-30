#include "stdafx.h"
#include "Clone_Pattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"
#include "SoundManager.h"

CClone_Pattern::CClone_Pattern()
{
}

CClone_Pattern::CClone_Pattern(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CClone_Pattern::~CClone_Pattern()
{
}

HRESULT CClone_Pattern::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fDelay = 0.f;
    m_fMoveDelay = 0.f;
    m_bSkill = false;
    m_bPosReset = false;
    m_bSound = false;
    m_vPillarPos[0] = _vec3(-72.5f, 49.f, 124.f);//위
    m_vPillarPos[1] = _vec3(-104.f, 49.f, 94.5f);//좌
    m_vPillarPos[2] = _vec3(-38.f, 49.f, 94.5f);//우
    m_iIndex = 0.f;
    return S_OK;
}

STATE CClone_Pattern::Update_State(const _float& fTimeDelta)
{
    m_fDelay += fTimeDelta;
    //std::random_device rd;
    //mt19937 engine(rd());    // MT19937 난수 엔진
   
    if (!m_bPosReset)
    {
        if ((1.5f >= fabs(-72.5f - m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x)) && (1.5f >= fabs(94.5f - m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z)))
        {
            m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = _vec3(-72.5f, 38.f, 94.5f);
            m_bPosReset = true;
        }
        m_vResetPos = _vec3(-72.5f, 38.f, 94.5f) - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
        D3DXVec3Normalize(&m_vResetPos, &m_vResetPos);
        m_pOwner->Get_Transform()->Translate(m_vResetPos);
        dynamic_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_CloneCount(0);

    }
    if ((m_bPosReset) && (!m_bSkill))
    {
        std::random_device rd;
        mt19937 engine(rd());
        uniform_int_distribution<__int64> distribution(0, 2); // 생성 범위
        auto generator = bind(distribution, engine);
        m_iIndex = generator();
        m_fDelay = 0.f;
        Engine::CGameObject* pGameObject = nullptr;
        Engine::CGameObject* pGameObject2 = nullptr;
        if (!m_bSkill)
        {
            switch (m_iIndex)
            {
            case 0:
                m_vTargetPos = m_vPillarPos[m_iIndex];
                pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_bMeteor(false);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_Dir(m_vPillarPos[1]);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
                Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

                pGameObject2 = CSkeletonKing_Clone::Create(m_pGraphicDev);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->Set_bMeteor(false);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->Set_Dir(m_vPillarPos[2]);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
                Engine::EventManager()->CreateObject(pGameObject2, LAYERTAG::GAMELOGIC);
                m_bSkill = true;
                break;
            case 1:
                m_vTargetPos = m_vPillarPos[m_iIndex];
                pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_bMeteor(false);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_Dir(m_vPillarPos[2]);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
                Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

                pGameObject2 = CSkeletonKing_Clone::Create(m_pGraphicDev);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->Set_bMeteor(false);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->Set_Dir(m_vPillarPos[0]);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
                Engine::EventManager()->CreateObject(pGameObject2, LAYERTAG::GAMELOGIC);
                m_bSkill = true;
                break;
            case 2:
                m_vTargetPos = m_vPillarPos[m_iIndex];
                pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_bMeteor(false);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->Set_Dir(m_vPillarPos[1]);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
                Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

                pGameObject2 = CSkeletonKing_Clone::Create(m_pGraphicDev);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->Set_bMeteor(false);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->Set_Dir(m_vPillarPos[0]);
                dynamic_cast<CSkeletonKing_Clone*>(pGameObject2)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
                Engine::EventManager()->CreateObject(pGameObject2, LAYERTAG::GAMELOGIC);
                m_bSkill = true;
                break;
            }
        }
    }
    if ((2.f< m_fDelay)&&(m_bSkill))
    {
        m_fMoveDelay += fTimeDelta;
        return Move_DIr();
    }
}

void CClone_Pattern::LateUpdate_State()
{
}

void CClone_Pattern::Render_State()
{
}

STATE CClone_Pattern::Move_DIr()
{
    m_vDir = m_vTargetPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
    D3DXVec3Normalize(&m_vDir, &m_vDir);
    m_pOwner->Get_Transform()->Translate(m_vDir);
    if (1.f < m_fMoveDelay)
    {
        m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = m_vTargetPos;
        m_fDelay = 0.f;
        m_fMoveDelay = 0.f;
        m_bSkill = false;
        m_bPosReset = false;
        m_bSound = false;
        return STATE::BOSS_IDLE;
    }
}

CClone_Pattern* CClone_Pattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CClone_Pattern* pState = new CClone_Pattern(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CClone_Pattern*>(pState);

        MSG_BOX("Fire State Failed");
    }
    return pState;
}

void CClone_Pattern::Free()
{
    __super::Free();
}
