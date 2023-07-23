#include "MeteorPh2.h"
#include "Export_Function.h"
#include "Player.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"
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
	m_bSkillStart = false;
	m_bCool = false;
    m_bSkill = false;
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
	Engine::CGameObject* pGameObject = nullptr;
	if (!m_bSkillStart)
	{
        Make_Clone();
		pGameObject = m_pGameObject = CBoss_MeteorCube::Create(m_pGraphicDev);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
		dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Set_State(STATE::BOSS_PH2SKILL5);
		dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Channeling_Begin();
		m_bSkillStart = true;
	}
	else if (m_bSkillStart && (10.1f < m_fChannel_Count))
	{
        m_fDelay = 0.f;
		m_bSkillStart = false;
		m_fChannel_Count = 0.f;
		return STATE::BOSS_PH2SKILL1;
	}
	else if (m_bSkillStart)
	{
		m_pOwner->Get_Transform()->Translate(_vec3(0.f, 0.5f * fTimeDelta, 0.f));
	}
    if (2.f < m_fDelay)
    {
        m_fDelay = 0.f;
        Make_LostSoul();
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


}

void CBoss_Meteor2Ph::Make_LostSoul()
{
		Engine::CGameObject* pGameObject = nullptr;

		pGameObject = CBossLostSoul::Create(m_pGraphicDev);
		dynamic_cast<CBossLostSoul*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		dynamic_cast<CBossLostSoul*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
}

CBoss_Meteor2Ph* CBoss_Meteor2Ph::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBoss_Meteor2Ph* pState = new CBoss_Meteor2Ph(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBoss_Meteor2Ph*>(pState);

		MSG_BOX("Fire State Failed");
	}
	return pState;
}

void CBoss_Meteor2Ph::Free()
{
	__super::Free();
}
