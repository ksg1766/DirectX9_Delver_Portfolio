#include "MeteorPh3.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_LostSoul.h"
#include "Skeleton.h"
#include "Boss_MeteorCube3Ph.h"
#include "MiniMeteor.h"
#include "Boss_WarningEff.h"
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
	m_fSpawnDelay = 0.f;
	m_fMiniDelay = 0.f;
	m_bSkill = false;
	m_bSkillStart = false;
	m_CautionCool = false;
	m_bCool = false;
	m_iSpawnX = 0;
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
	m_fMiniDelay+= fTimeDelta;
	Make_MiniMeteor();
	if (!m_bSkillStart)
	{
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CBoss_MeteorCube3Ph::Create(m_pGraphicDev);
		static_cast<CBoss_MeteorCube3Ph*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
		static_cast<CBoss_MeteorCube3Ph*>(pGameObject)->Set_State(STATE::BOSS_PH3SKILL5);
		static_cast<CBoss_MeteorCube3Ph*>(pGameObject)->Channeling_Begin();
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_bSkillStart = true;
	}
	else if (m_bSkillStart && (20.1f < m_fChannel_Count))
	{
		m_fDelay = 0.f;
		m_bSkillStart = false;

		m_fChannel_Count = 0.f;
		static_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_Phase(BOSSPHASE::LASTPHASE);
		static_cast<CSkeletonKing*>(Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Set_3Phase(false);
		return STATE::BOSS_IDLE;
	}
	if (2.0f < m_fSpawnDelay)
	{
		m_fSpawnDelay = 0.f;
		Make_LostSoul();
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
	m_fChannel_Count = 0.f;
	m_bSkill = false;
	m_bSkillStart = false;
	m_vLaunchPos[0] = _vec3(16.f, 0.f, 0.f);
	m_vLaunchPos[1] = _vec3(10.f, 5.f, 0.f);
	m_vLaunchPos[2] = _vec3(0.f, 10.f, 0.f);
	m_vLaunchPos[3] = _vec3(-10.f, 5.f, 0.f);
	m_vLaunchPos[4] = _vec3(-16.f, 0.f, 0.f);
}

void CMeteorPh3::Make_LostSoul()
{
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CBossLostSoul::Create(m_pGraphicDev);
	static_cast<CBossLostSoul*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS]= m_vLaunchPos[2];
	static_cast<CBossLostSoul*>(pGameObject)->Set_Target(m_vSpawnPos[0]);
	Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	m_fSpawnDelay = 0.f;
}

void CMeteorPh3::Make_MiniMeteor()
{
	Engine::CGameObject* pGameObject = nullptr;
	if ((0.4f > m_fMiniDelay) && (!m_CautionCool))
	{
		std::random_device rd;
		mt19937 engine(rd());
		uniform_int_distribution<__int64> distribution(-18, 18); // 생성 범위
		auto generator = bind(distribution, engine);
		m_iSpawnX = generator();
		pGameObject = CBoss_WarningEff::Create(m_pGraphicDev);
		static_cast<CBoss_WarningEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_iSpawnX, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y + 5.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + 5.f);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_fDelay = 0.f;
		m_CautionCool = true;
	}
	if ((0.4f < m_fMiniDelay) && (m_CautionCool) && (!m_bCool))
	{
		pGameObject = CMiniMeteor::Create(m_pGraphicDev);
		static_cast<CMiniMeteor*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_iSpawnX, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y + 5.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + 5.f);
		static_cast<CMiniMeteor*>(pGameObject)->Set_Dir(_vec3(0.f, 0.f, 0.3f));
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_bCool = true;
	}
	if ((0.4f < m_fMiniDelay) && (m_bCool) && (m_CautionCool))
	{
		m_fDelay = 0.f;
		m_bCool = false;
		m_CautionCool = false;
		m_fMiniDelay = 0.f;
	}
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
