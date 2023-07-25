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
	m_iSkillCount = 1;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_bCool = false;
	return S_OK;
}

STATE CLightningPattern2::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if ((0.5f > m_fDelay) && (!m_bCool))
	{
		m_bCool = true;
		m_vPlayerPos = Engine::SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
		pGameObject = CBoss_CautionEff::Create(m_pGraphicDev);
		for (int i = 0; i < 10; ++i)
		{
			dynamic_cast<CBoss_CautionEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_vPlayerPos.x, 35.f + (i * 2), m_vPlayerPos.z);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
	}
	else if (0.5f < m_fDelay)
	{
		for (int i = 0; i < 10; ++i)
		{
			pGameObject = CBoss_Lightning::Create(m_pGraphicDev);
			dynamic_cast<CBoss_Lightning*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(m_vPlayerPos.x, 35.f + (i * 2), m_vPlayerPos.z);
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_fDelay = 0.f;
		++m_iSkillCount;
		m_bCool = false;
	}
	if (5 < m_iSkillCount)
	{
		m_bCool = false;
		m_iSkillCount = 0;
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
