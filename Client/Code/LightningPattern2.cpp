#include "LightningPattern2.h"
#include "Export_Function.h"
#include "Boss_Lightning.h"
#include "Boss_CautionEff.h"
#include "SkeletonKing.h"

CLightningPattern2::CLightningPattern2()
{
}

CLightningPattern2::CLightningPattern2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CLightningPattern2::~CLightningPattern2()
{
}

HRESULT CLightningPattern2::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDuration = 0.f;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_fRandomX = 0.f;
	m_fRandomZ = 0.f;
	m_bCool = false;
	m_vCrossDir[0] = _vec3(-2.f, 0.f, 0.f);
	m_vCrossDir[1] = _vec3(0.f, 0.f, -2.f);
	m_vCrossDir[2] = _vec3(2.f, 0.f, 0.f);
	m_vCrossDir[3] = _vec3(0.f, 0.f, 2.f);
	return S_OK;
}

STATE CLightningPattern2::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	m_fDuration += fTimeDelta;
	if ((0.1f > m_fDelay) && (!m_bCool))
	{
		std::random_device rd;
		mt19937 engineRandom(rd());
		uniform_int_distribution<__int64> distributionX(-18, 18);
		auto generatorX = bind(distributionX, engineRandom);
		m_fRandomX = generatorX();

		uniform_int_distribution<__int64> distributionZ(5.f, 30.f); // 생성 범위
		auto generatorZ = bind(distributionZ, engineRandom);
		m_fRandomZ = generatorZ();

		m_vPlayerPos = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
		for (int i = 0; i < 10; ++i)
		{
			pGameObject = CBoss_CautionEff::Create(m_pGraphicDev);
			dynamic_cast<CBoss_CautionEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_fRandomX, 35.f + (i * 2), m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + m_fRandomZ);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_bCool = true;
	}
	else if (0.5f <= m_fDelay)
	{
		for (int i = 0; i < 10; ++i)
		{
				pGameObject = CBoss_Lightning::Create(m_pGraphicDev);
				dynamic_cast<CBoss_Lightning*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_pOwner->Get_Transform()->m_vInfo[INFO_POS].x + m_fRandomX, 35.f + (i * 2), m_pOwner->Get_Transform()->m_vInfo[INFO_POS].z + m_fRandomZ);
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_fDelay = 0.f;
		m_bCool = false;
	}
	if (15.f < m_fDuration)
	{
		m_bCool = false;
		m_fDuration = 0.f;
		m_fDelay = 0.f;
		return STATE::BOSS_PH3SKILL4;
	}
}

void CLightningPattern2::LateUpdate_State()
{
}

void CLightningPattern2::Render_State()
{
}

CLightningPattern2* CLightningPattern2::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CLightningPattern2* pState = new CLightningPattern2(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CLightningPattern2*>(pState);

		MSG_BOX("Fire State Failed");
	}
	return pState;
}

void CLightningPattern2::Free()
{
	__super::Free();
}
