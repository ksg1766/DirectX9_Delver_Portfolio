#include "Clone_Pattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"

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
    m_fPatternDelay = 0.f;
    m_bSkill = false;
    m_vPillarPos[0] = _vec3(-72.5f, 49.f, 124.f);//위
    m_vPillarPos[1] = _vec3(-104.f, 49.f, 94.5f);//좌
    m_vPillarPos[2] = _vec3(-38.f, 49.f, 94.5f);//우

    return S_OK;
}

STATE CClone_Pattern::Update_State(const _float& fTimeDelta)
{
    mt19937 engine((_uint)time(NULL));           // MT19937 난수 엔진
    uniform_int_distribution<__int64> distribution(0, 2); // 생성 범위
    auto generator = bind(distribution, engine);
    switch (generator())
    {
    case 0:
        if (!m_bSkill)
        {
            m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = (m_vPillarPos[0]);
            Engine::CGameObject* pGameObject = nullptr;
            pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
            dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_vPillarPos[1]);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
            dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_vPillarPos[2]);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);

        }
        break;
    case 1:
        if (!m_bSkill)
        {
            m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = (m_vPillarPos[1]);
            Engine::CGameObject* pGameObject = nullptr;
            pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
            dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_vPillarPos[0]);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
            dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_vPillarPos[2]);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        break;
    case 2:
        if (!m_bSkill)
        {
            m_pOwner->Get_Transform()->m_vInfo[INFO_POS] = (m_vPillarPos[2]);
            Engine::CGameObject* pGameObject = nullptr;
            pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
            dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_vPillarPos[1]);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
            pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
            dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (m_vPillarPos[0]);
            Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        }
        break;
    }

	return STATE::BOSS_IDLE;
}

void CClone_Pattern::LateUpdate_State()
{
}

void CClone_Pattern::Render_State()
{
}

void CClone_Pattern::Create_Clone(_int _iNum)
{
    if (!m_bSkill)
    {
        Engine::CGameObject* pGameObject = nullptr;
        pGameObject = CSkeletonKing_Clone::Create(m_pGraphicDev);
        dynamic_cast<CSkeletonKing_Clone*>(pGameObject)->m_pTransform->Translate(m_vPillarPos[_iNum]);
        m_bSkill = true;
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
