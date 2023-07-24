#include "CrossPattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"
#include "Boss_Lightning.h"
CCrossPattern::CCrossPattern()
{
}

CCrossPattern::CCrossPattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CCrossPattern::~CCrossPattern()
{
}

HRESULT CCrossPattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_iSkillCount = 0;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_bCool = false;
	m_vHeight = _vec3(0.f, 10.f, 0.f);
	m_vCrossDir[0] = _vec3(-2.f, 0.f, 0.f);
	m_vCrossDir[1] = _vec3(0.f, 0.f, -2.f);
	m_vCrossDir[2] = _vec3(2.f, 0.f, 0.f);
	m_vCrossDir[3] = _vec3(0.f, 0.f, 2.f);
	return S_OK;
}

STATE CCrossPattern::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if (0.1f< m_fDelay)
	{
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pGameObject = CBoss_Lightning::Create(m_pGraphicDev);
				dynamic_cast<CBoss_Lightning*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = (_vec3(-72.5f, 35.f + (i * 2), 94.5f) + (m_vCrossDir[j] * (m_iSkillCount)));
				Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
			}
		}
		m_fDelay = 0.f;
		++m_iSkillCount;
	}
	if (10 < m_iSkillCount)
	{
		m_iSkillCount = 0.f;
		//return STATE::BOSS_IDLE;
		return STATE::BOSS_PH2SKILL3;
	}
}

void CCrossPattern::LateUpdate_State()
{
}

void CCrossPattern::Render_State()
{
}

CCrossPattern* CCrossPattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CCrossPattern* pState = new CCrossPattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CCrossPattern*>(pState);

		MSG_BOX("Fire State Failed");
	}
	return pState;
}

void CCrossPattern::Free()
{
	__super::Free();
}
