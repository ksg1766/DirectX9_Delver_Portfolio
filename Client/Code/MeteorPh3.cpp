#include "MeteorPh3.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_LostSoul.h"
#include "Skeleton.h"
#include "Boss_MeteorCube3Ph.h"
CMeteorPh3::CMeteorPh3()
{
}

CMeteorPh3::CMeteorPh3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CMeteorPh3::~CMeteorPh3()
{
}

HRESULT CMeteorPh3::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_fSpawnDelay = 0.f;
	m_bSkill = false;
	m_bSkillStart = false;
	m_vSpawnPos[0] = _vec3(-72.f, 35.f, -85.f);
	m_vSpawnPos[1] = _vec3(-64.f, 35.f, -85.f);
	m_vSpawnPos[2] = _vec3(-80.f, 35.f, -85.f);
	return S_OK;
}

STATE CMeteorPh3::Update_State(const _float& fTimeDelta)
{
	m_fDelay += fTimeDelta;
	m_fChannel_Count += fTimeDelta;
	m_fSpawnDelay += fTimeDelta;
	if (!m_bSkillStart)
	{
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CBoss_MeteorCube3Ph::Create(m_pGraphicDev);
		dynamic_cast<CBoss_MeteorCube3Ph*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
		dynamic_cast<CBoss_MeteorCube3Ph*>(pGameObject)->Set_State(STATE::BOSS_PH2SKILL5);
		dynamic_cast<CBoss_MeteorCube3Ph*>(pGameObject)->Channeling_Begin();
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_bSkillStart = true;
	}
	else if (m_bSkillStart && (12.1f < m_fChannel_Count))
	{
		m_fDelay = 0.f;
		m_bSkillStart = false;

		m_fChannel_Count = 0.f;
		return STATE::BOSS_IDLE;
	}
	else if (m_bSkillStart)
	{
		if (2.0f < m_fDelay)
		{
			m_fDelay = 0.f;
			Make_LostSoul();
		}
		if (3.5f< m_fSpawnDelay)
		{
			m_fSpawnDelay = 0.f;
			for (int i = 1; i < 3; ++i)
			{
				Engine::CGameObject* pGameObject = nullptr;
				pGameObject = CSkeleton::Create(m_pGraphicDev);
				dynamic_cast<CSkeleton*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_vSpawnPos[i];
				dynamic_cast<CSkeleton*>(pGameObject)->Get_BasicStat()->Get_Stat()->fHP = 2.f;
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
		}
	}

}

void CMeteorPh3::LateUpdate_State()
{
}

void CMeteorPh3::Render_State()
{
}

void CMeteorPh3::Reset_Member()
{
	m_fDelay = 0.f;
	m_fSpawnDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_fChannel_Count = 0.f;
	m_bSkill = false;
	m_bSkillStart = false;
}

void CMeteorPh3::Make_LostSoul()
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CBossLostSoul::Create(m_pGraphicDev);
	dynamic_cast<CBossLostSoul*>(pGameObject)->Set_Target(m_vSpawnPos[0]);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	m_fDelay = 0.f;
	m_bSkill = true;
}

CMeteorPh3* CMeteorPh3::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMeteorPh3* pState = new CMeteorPh3(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMeteorPh3*>(pState);

		MSG_BOX("Fire State Failed");
	}
	return pState;
}

void CMeteorPh3::Free()
{
	__super::Free();
}
