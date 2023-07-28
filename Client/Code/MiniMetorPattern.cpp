#include "MiniMetorPattern.h"
#include "MiniMeteor.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_WarningEff.h"
#include "Boss_Lightning.h"
#include "Boss_CautionEff.h"
CMiniMetorPattern::CMiniMetorPattern()
{
}

CMiniMetorPattern::CMiniMetorPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CMiniMetorPattern::~CMiniMetorPattern()
{
}

HRESULT CMiniMetorPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_iSkillCount = 0;
	m_fDelay = 0.f;
	m_bCool = false;
	m_CautionCool = false;
	m_fThunderDelay = 0.f;
	m_iSpawnX = 0;
	return S_OK;
}

STATE CMiniMetorPattern::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	m_fThunderDelay += fTimeDelta;
	Make_Thunder();
	if ((0.4f < m_fDelay) && (!m_CautionCool))
	{
		std::random_device rd;
		mt19937 engineRandom(rd());
		uniform_int_distribution<__int64> distributionX(-18, 18);
		auto generatorX = bind(distributionX, engineRandom);
		m_iSpawnX = generatorX();

		pGameObject = CBoss_WarningEff::Create(m_pGraphicDev);
		dynamic_cast<CBoss_WarningEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_iSpawnX, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y + 5.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + 5.f);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_fDelay = 0.f;
		m_CautionCool = true;
	}
	if ((0.4f < m_fDelay) && (m_CautionCool)&&(!m_bCool))
	{
		pGameObject = CMiniMeteor::Create(m_pGraphicDev);
		dynamic_cast<CMiniMeteor*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_iSpawnX, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y + 5.f, m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + 5.f);
		dynamic_cast<CMiniMeteor*>(pGameObject)->Set_Dir(_vec3(0.f, 0.f, 0.3f));
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_bCool = true;
	}
	if ((0.4f < m_fDelay)&&(m_bCool)&&(m_CautionCool))
	{
		m_fDelay = 0.f;
		m_bCool = false;
		m_CautionCool = false;
		++m_iSkillCount;
	}
	if (20 <= m_iSkillCount)
	{
		m_fDelay = 0.f;
		m_bCool = false;
		m_iSkillCount = 0;
		return STATE::BOSS_IDLE;
	}
}

void CMiniMetorPattern::LateUpdate_State()
{
}

void CMiniMetorPattern::Render_State()
{
}

void CMiniMetorPattern::Make_Thunder()
{
	Engine::CGameObject* pGameObject = nullptr;

	if ((0.5f > m_fThunderDelay) && (!m_bThunderCool))
	{
		m_bThunderCool = true;
		m_vPlayerPos = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
		for (int i = 0; i < 10; ++i)
		{
			pGameObject = CBoss_CautionEff::Create(m_pGraphicDev);
			dynamic_cast<CBoss_CautionEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_vPlayerPos.x, 35.f + (i * 2), m_vPlayerPos.z);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
	}
	else if (1.f < m_fThunderDelay)
	{
		for (int i = 0; i < 10; ++i)
		{
			pGameObject = CBoss_Lightning::Create(m_pGraphicDev);
			dynamic_cast<CBoss_Lightning*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_vPlayerPos.x, 35.f + (i * 2), m_vPlayerPos.z);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_fThunderDelay = 0.f;
		m_bThunderCool = false;
	}
}

CMiniMetorPattern* CMiniMetorPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CMiniMetorPattern* pState = new CMiniMetorPattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CMiniMetorPattern*>(pState);

		MSG_BOX("MiniMeteor State Failed");
	}
	return pState;
}

void CMiniMetorPattern::Free()
{
	__super::Free();
}
