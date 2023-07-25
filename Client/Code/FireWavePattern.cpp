#include "FireWavePattern.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "SkeletonKing_Clone.h"
#include "BossFireWave.h"
#include "Boss_WarningEff.h"
CFireWavePattern::CFireWavePattern()
{
}

CFireWavePattern::CFireWavePattern(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CFireWavePattern::~CFireWavePattern()
{
}

HRESULT CFireWavePattern::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_bCool = false;
	m_bWarning = false;
	m_iCount = 0;
	m_vWavePos = _vec3(-24.f, 0.f, 0.f);

	return S_OK;
}

STATE CFireWavePattern::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if (!m_bWarning)
	{
		for (int i = 0; i < 16; ++i)
		{
			pGameObject = CBoss_WarningEff::Create(m_pGraphicDev);
			dynamic_cast<CBossFireWave*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(-72.5f, 36.f, 94.5f) + (_vec3(-30 + (i * 4), -2.f, 0.f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_bWarning = true;
	}
	if (1.5f < m_fDelay)
	{
		for (int i = 0; i < 16; ++i)
		{
			pGameObject = CBossFireWave::Create(m_pGraphicDev);
			dynamic_cast<CBossFireWave*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(-72.5f, 36.f, 94.5f) + (_vec3(-30 + (i * 4), -2.f, 0.f));
			dynamic_cast<CBossFireWave*>(pGameObject)->Set_Dir(_vec3(0.f, 0.f, -1.f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_fDelay = 0.f;
		++m_iCount;
	}
	if (3 < m_iCount)
	{
		m_bWarning = false;
		m_iCount = 0;
		return STATE::BOSS_IDLE;
	}
}

void CFireWavePattern::LateUpdate_State()
{
}

void CFireWavePattern::Render_State()
{
}

CFireWavePattern* CFireWavePattern::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CFireWavePattern* pState = new CFireWavePattern(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CFireWavePattern*>(pState);

		MSG_BOX("FireWave State Failed");
	}
	return pState;
}

void CFireWavePattern::Free()
{
	__super::Free();
}
